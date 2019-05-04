// pd_client.cpp
// Class pd_client.
// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <pd_client.h>
#include <pd_message.h>
#include <pd_tools.h>

using boost::asio::ip::tcp;

// Constructor
pd_client::pd_client() :
	io_service_(),
    socket_(io_service_)
{

}

/// Destructor
pd_client::~pd_client() {
    close();
}

// Creates a connection to the server.
// address - remote host address.
// port - server port.
void pd_client::connect(const std::string& address, const std::string& port) {
    if(connected() || running) return;
	address_ = address;
	port_ = port;
	do_connect();
}

// Closes the connection.
void pd_client::close() {
    try {
        if(socket_.is_open()) {
            DBG("close socket");
            io_service_.post([this]() {socket_.close();});
            bth.join();
            DBG("close OK");
        }
    } catch (...) {
        DBG("close FAIL - exception");
    }
}

// Logging into the server.
// user - user name.
// paswd - user password.
void pd_client::login(const std::string& user, const std::string& paswd) {
	if(!connected()) return;
	user_ = user;
	password_ = paswd;
	if(user_.empty() || password_.empty()) return;
	pd_message msg("LOGIN|" + user_+ "|" + password_);
	write(msg);
}

// Logout from the server.
void pd_client::logout() {
	pd_message msg("LOGOUT");
	write(msg);
}

// Adds a new event.
// priority - event priority.
// info - information about the event.
void pd_client::add_new_event(int priority, const std::string& info) {
	char buf[1024];
	std::sprintf(buf, "NEW|%d|%s", priority, info.c_str());
	pd_message msg(buf);
	write(msg);
}

// Get all events.
// Events are received by on_message callback.
void pd_client::get_all_events() {
	pd_message msg("ALL");
	write(msg);
}

// Get last event.
// The event is received by on_message callback.
void pd_client::get_last_event() {
    pd_message msg("LAST");
    write(msg);
}

// Direct transmission of messages to the server.
// msg - pd_message.
void pd_client::write(pd_message& msg) {
	msg.encrypt(password_);
	io_service_.post([this, msg]()
			{
				bool write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
				if (!write_in_progress)
				{
					do_write();
				}
			});
}

// Sets on_message handler.
// It is used to receive messages from the server.
// handler - function.
void pd_client::set_on_message(pd_on_message_func handler) {
	on_message = handler;
}

// Returns whether the client is connected.
// return true if connected.
bool pd_client::connected() {
    return socket_.is_open();
}

// Returns whether the client is logged in.
// return true if logged in.
bool pd_client::loggedin() {
	return (!password_.empty()) && connected();
}

// Create connection.
void pd_client::do_connect() {
    running = false;
    io_service_.reset();
    write_msgs_.clear();

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(tcp::v4(), address_.c_str(), port_.c_str());
    tcp::resolver::iterator endpoint_iterator;
    try {
        endpoint_iterator = resolver.resolve(query);
    }
    catch (boost::system::system_error &e) {
        DBG("Resolver error: %s", e.what());
        if(socket_.is_open()) socket_.close();
        return;
    }

    DBG("Client connect...");
    boost::asio::async_connect(socket_, endpoint_iterator,
                              [this](boost::system::error_code ec, tcp::resolver::iterator /*it*/)
    {
        if (!ec) {
            do_read_header();
        }
        else {
            DBG("Connection error: %s [%d]", ec.message().c_str(), ec.value());
            if(socket_.is_open()) socket_.close();
            return;
        }
    });

    if(!socket_.is_open()) {
        DBG("Client connection: FAILED");
        return;
    }

    DBG("Client connected - run service.");
        bth = boost::thread([&]()
        {
            DBG("Thread running...");
            try {
                running = true;
                io_service_.run();
            } catch (...) {
                DBG("Exception in the thread.");
            }
            running = false;
            if(socket_.is_open()) socket_.close();
            DBG("Exit from the thread.");
        });

    // wait for the thread running
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

// Reads message header.
void pd_client::do_read_header() {
	boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(),
					pd_message::header_length),
                    [this](boost::system::error_code ec, std::size_t /*len*/)
					{
						if (!ec && read_msg_.decode_header()) {
							do_read_body();
						}
						else {
                            DBG("Read error: %s [%d]", ec.message().c_str(), ec.value());
                            if(socket_.is_open()) socket_.close();
						}
					});
}

// Reads message body.
void pd_client::do_read_body() {
	boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this](boost::system::error_code ec, std::size_t /*len*/)
			{
				if (!ec) {
					read_msg_.decrypt(password_);
					if(on_message) {
						std::vector<std::string> v = split_string(read_msg_.str(), "|");
						std::string cmd = v[0];
						v.erase(v.begin());
                        on_message(cmd, v); // call on_message handler
					}
					do_read_header();
				}
				else {
                    DBG("Read error: %s [%d]", ec.message().c_str(), ec.value());
                    if(socket_.is_open()) socket_.close();
				}
			});
}

// Writes message.
void pd_client::do_write() {
	boost::asio::async_write(socket_,
			boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
                    [this](boost::system::error_code ec, std::size_t /*len*/)
					{
						if (!ec) {
							write_msgs_.pop_front();
							if (!write_msgs_.empty()) {
								do_write();
							}
						}
						else {
                            DBG("Write error: %s [%d]", ec.message().c_str(), ec.value());
                            write_msgs_.clear();
                            if(socket_.is_open()) socket_.close();
						}
					});
}



