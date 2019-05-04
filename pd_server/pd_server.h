/// \file pd_server.h
/// \brief Class pd_server.
///
/// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#ifndef PD_SERVER_H
#define PD_SERVER_H

#include <string>
#include <deque>
#include <set>
#include <boost/asio.hpp>
#include <pd_message.h>

//-------------------------------------------------------------------
/// Class pd_user - Abstrac base class for pd_session.
class pd_user {
public:
    /// Virtual destructor.
    virtual ~pd_user() {}

	/// Abstract procedure for delivering messages to the user.
    virtual void deliver(const pd_message& msg) = 0;

	bool loggedin = false;	///< \brief Whether the user is logged in.
	std::string name;		///< \brief User name.
	std::string password;	///< \brief User password.
};

typedef std::shared_ptr<pd_user> pd_user_ptr;

//-------------------------------------------------------------------
/// Class pd_room - Virtual session room with clients.
class pd_room {
public:
	/// Adds the user to the session room.
	void join(pd_user_ptr user);

	/// Removes the user from the session room.
	void leave(pd_user_ptr user);

	/// Delivers messages to all users.
    void deliver(const pd_message& msg);

private:
	std::set<pd_user_ptr> users_;	///< \brief User collection.
};

//-------------------------------------------------------------------
/// Class pd_session - Session of communication with clients.
class pd_session: public pd_user,
		public std::enable_shared_from_this<pd_session> {
public:
	/// Session constructor.
	pd_session(boost::asio::ip::tcp::socket socket, pd_room& room);

	/// Starts the session with the client.
	void start();

	/// Closes the session with the client.
	void close();

	/// \brief Delivers message to client.
	/// The message is added to the queue.
    void deliver(const pd_message& msg);

private:
	/// Reads the message header.
	void do_read_header();

	/// Reads the message body.
	void do_read_body();

	/// Sends a message from the beginning of the queue.
	void do_write();

	/// Checks the login user
	bool check_login();

	/// Parses received message
	void parse_message();

private:
	boost::asio::ip::tcp::socket socket_;	///< TCP socket.
	pd_room& room_;							///< Reference to connection room.
	pd_message read_msg_;					///< Current reading message.
    std::deque<pd_message> write_msgs_;     ///< Message queue.
};

//-------------------------------------------------------------------
/// Class pd_server - "Packet Data Server"
class pd_server {
public:
	/// Server constructor.
	pd_server(const std::string& port);

	/// Server destructor.
	~pd_server();

	/// Runs the server.
	void run();

private:
	/// Accepts the connection with the client
	void do_accept();

private:
	int port_;									///< Server port.
	boost::asio::io_service io_service_;		///< TCP asynchronous service.
	boost::asio::ip::tcp::acceptor acceptor_;	///< TCP acceptor.
	boost::asio::ip::tcp::socket socket_;		///< TCP socket.
    pd_room room_;                              ///< Sessions room
};

#endif // PD_SERVER_H_

