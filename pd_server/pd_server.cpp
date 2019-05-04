// pd_server.cpp
// Class pd_server
// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#include <cstdlib>
#include <csignal>
#include <ctime>
#include <deque>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <functional>
#include <chrono>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pd_server.h>
#include <pd_message.h>
#include <pd_sqlite.h>
#include <pd_tools.h>

using boost::asio::ip::tcp;

//-------------------------------------------------------------------
// class pd_room
//-------------------------------------------------------------------

// Adds the user to the session room.
void pd_room::join(pd_user_ptr user) {
    users_.insert(user);
}

// Removes the user from the session room.
void pd_room::leave(pd_user_ptr user) {
    users_.erase(user);
}

// Delivers messages to all users.
void pd_room::deliver(const pd_message& msg) {
    for (auto user : users_)
        user->deliver(msg);
}

//-------------------------------------------------------------------
// class pd_session
//-------------------------------------------------------------------

// Session constructor
pd_session::pd_session(tcp::socket socket, pd_room& room) :
		socket_(std::move(socket)), room_(room) {
}

// Starts the session with the client.
void pd_session::start() {
	do_read_header();
}

// Closes the session with the client.
void pd_session::close() {
	room_.leave(shared_from_this());
    read_msg_ = "";
	socket_.close();
}

// Delivers message to client.
// The message is added to the queue.
void pd_session::deliver(const pd_message& msg) {
    DBG("deliver");
    bool write_in_progress = !write_msgs_.empty();
	pd_message enc_msg = msg;
	enc_msg.encrypt(password);
	write_msgs_.push_back(enc_msg);
    if (!write_in_progress) {
		do_write();
    }
}

// Reads the message header.
void pd_session::do_read_header() {
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(),
					pd_message::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*len*/)
			{
				if (!ec && read_msg_.decode_header()) {
                    do_read_body();
				}
				else {
					room_.leave(shared_from_this());
                    DBG("The connection with user '%s' has been closed.", name.c_str());
				}
			});
}

// Reads the message body.
void pd_session::do_read_body() {
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*len*/)
			{
				if (!ec) {
					if(!loggedin) {
                        if(check_login()) { // authorization ACCEPTED - send response
							loggedin = true;
                            std::cout << "User: " << name << " - login" << std::endl;
							room_.join(shared_from_this());
							deliver(pd_message("LOGIN|ACCEPTED"));
						}
                        else { // authorization REJECTED - send response and close connection
							pd_message msg("LOGIN|REJECTED");
							boost::asio::async_write(socket_,
									boost::asio::buffer(msg.data(), msg.length()),
                                    [this, self](boost::system::error_code /*ec*/, std::size_t /*len*/)
                                    {close();});
						}
					}
                    else { // normal operation - parse message
                        read_msg_.decrypt(password);
						parse_message();
					}
					do_read_header(); // get header from next message
				}
				else {
					room_.leave(shared_from_this());
                    DBG("The connection with user '%s' has been closed. boost error code: %s",
                        name.c_str(), ec.message().c_str());
				}
			});
}

// Sends a message from the beginning of the queue.
void pd_session::do_write() {
    DBG("do_write");
	auto self(shared_from_this());
	boost::asio::async_write(socket_,
			boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*len*/)
            {
				if (!ec) {
                    DBG("write_msgs_.size() = %ld", write_msgs_.size());
                    write_msgs_.pop_front();
					if (!write_msgs_.empty()) {
						do_write();
					}
				}
				else {
                    //write_msgs_.clear();
					room_.leave(shared_from_this());
                    DBG("The connection with user '%s' has been closed. boost error code: %s",
                        name.c_str(), ec.message().c_str());
				}
			});
}

// Checks the login user
bool pd_session::check_login() {
	std::ifstream file("secret.pdb");
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			std::vector<std::string> v = split_string(line, "|"); // user_name|password
			if (v.size() != 2)
				continue;
			pd_message msg = read_msg_;
			if(!msg.decrypt(v[1])) continue;
			std::vector<std::string> decv = split_string(msg.str(), "|"); // LOGIN|user_name|password
			if(decv.size() != 3) continue;
			if(decv[0]=="LOGIN" && decv[1]==v[0] && decv[2]==v[1]) {
				name = decv[1];
				password = decv[2];
                DBG("LOGIN - OK");
				return true;
			}
		}
		file.close();
	}
    DBG("LOGIN - FAIL");
	return false;
}

// Parses received message
void pd_session::parse_message() {
    DBG("read_msg_: %s [size=%ld]", read_msg_.str().c_str(), read_msg_.body_length());
    // split message (like: aaa|bbb|ccc) to vector v
	std::vector<std::string> v = split_string(read_msg_.str(), "|");
    if (v.size() < 1) {
        DBG("NEW: data incomplete!");
        return;
    }

    if (v[0] == "ALL") { // get all events from database
		pd_sqlite db;
		char* err_str;
		db.open();
		db.exec("SELECT * FROM events ORDER BY timestamp;",
                [](void*data,int count,char**value, char**/*colname*/)->int
				{
					pd_session* session = static_cast<pd_session*>(data);
					std::string s = "ALL";
					for(auto i=0; i<count; i++) {
						s += std::string("|") + value[i];
					}
					pd_message msg(s.c_str());
                    session->deliver(msg); // send response
					return 0;
				}, this, &err_str);
		if (err_str)
            DBG("ALL: %s\n", err_str);
		db.close();
	}
    else if (v[0] == "LAST") { // get last event from database
		pd_sqlite db;
		char* err_str;
		db.open();
		db.exec("SELECT * FROM events ORDER BY timestamp DESC LIMIT 1;",
                [](void*data,int count,char**value, char**/*colname*/)->int
				{
					pd_session* session = static_cast<pd_session*>(data);
					std::string s = "LAST";
					for(auto i=0; i<count; i++) {
						s += std::string("|") + value[i];
					}
					pd_message msg(s.c_str());
                    session->deliver(msg); // send response
					return 0;
				}, this, &err_str);
		if (err_str)
            DBG("LAST: %s\n", err_str);
		db.close();
	}
    else if (v[0] == "NEW") { // add new event to database
		if (v.size() != 3) {
            DBG("NEW: data incomplete!");
			return;
		}
		pd_sqlite db;
        char* err_str = nullptr;
        long int timestamp = std::time(nullptr);
        char buf[1024];
		sprintf(buf,
				"INSERT INTO events(timestamp,priority,info) VALUES(%ld,'%s','%s');",
				timestamp, v[1].c_str(), v[2].c_str());
		db.open();
        db.exec(buf, nullptr, nullptr, &err_str);
		db.close();
		if (err_str) {
            DBG("NEW: %s\n", err_str);
			return;
		}
        // send message to all users about new event
		sprintf(buf, "NEW|%ld|%s|%s", timestamp, v[1].c_str(), v[2].c_str());
		room_.deliver(pd_message(buf));
	} else if (v[0] == "LOGOUT") { // logout
		std::cout << "User: " << name << " - logout" << std::endl;
		close();
	}
}

//-------------------------------------------------------------------
// class pd_server
//-------------------------------------------------------------------

// Server constructor.
pd_server::pd_server(const std::string& port) :
    port_(std::stoi(port)),
	io_service_(),
    acceptor_(io_service_, tcp::endpoint(tcp::v4(), static_cast<unsigned short>(port_))), socket_(io_service_)
{
	do_accept();
}

// Server destructor.
pd_server::~pd_server() {
}

/// Runs the server.
void pd_server::run() {
	io_service_.run();
}

/// Accepts the connection with the client
void pd_server::do_accept() {
	acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
			{
				if (!ec)
				{
					std::make_shared<pd_session>(std::move(socket_), room_)->start();
				}

				do_accept();
			});
}

//----------------------------------------------------------------------
// Main program function.
//----------------------------------------------------------------------

int main(int argc, char* argv[]) {
    // Checking if there is a file with users 'secret.pdb'.
    // If not, it creates a new one with the default user 'admin' and the password 'admin'.
    std::string pdb_file = boost::filesystem::current_path().string() + "/secret.pdb";
    if (!boost::filesystem::exists(pdb_file)) {
        std::cout << "File '" << pdb_file << "' not exists!" << std::endl;
        std::cout << "Create new one with the default user 'admin' and the password 'admin'." << std::endl;
        std::cout << "If you want to add a new user or change the password, you must edit the 'secret.pdb' file." << std::endl
                  << "Each line of the file must contain 'user_name|password'" << std::endl;
        std::ofstream file(pdb_file.c_str());
        if(!file) {
            std::cout << "ERROR! - Can't open " << pdb_file << std::endl;
            return 1;
        }
        file << "admin|admin" << std::endl;
        file.close();
    }

    int port = 11111; // default port value

	// Check program options
	namespace po = boost::program_options;
	po::options_description desc("Options");
    desc.add_options()
            ("debug,d",  "Enable debug info.")
            ("help,h" ,  "Print help.")
            ("port,p" ,  po::value<int>(), "Port (default: 11111).");

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc),vm); // can throw

        // --help,-h  option: prints help
		if (vm.count("help")) {
			std::cout << "Server command line parameters." << std::endl
					<< desc << std::endl;
			return 0;
		}
		po::notify(vm); // throws on error, so do after help in case
						// there are any problems

        // --debug,-d  option: enables printing debug info
		if (vm.count("debug")) {
			pd_debug_info = true;
			std::cout << "Debug info enabled." << std::endl;
            DBG("Debug info enabled.");
		}

        // --port,-p  option: server port
		if (vm.count("port")) {
			port = vm["port"].as<int>();
			std::cout << "Set port to: " <<  port << std::endl;
		}
	}
	catch(po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

    //----------------
    // Run the server
    //----------------
	try {
        // Install the system signals handlers
		std::signal(SIGINT,  [](int sig){throw sig;});
		std::signal(SIGTERM, [](int sig){throw sig;});
#ifdef __linux__
        std::signal(SIGKILL, [](int sig){throw sig;});
#endif

        // Create the server
		pd_server server(std::to_string(port));

		// Run the server
        std::cout << "Server start on the port: " << port << std::endl;
		server.run();
	} catch (std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 2;
	} catch (int val) {
        std::cout << "Receive signal: " << val << " - close server" << std::endl;
	}

    return 0;
}
