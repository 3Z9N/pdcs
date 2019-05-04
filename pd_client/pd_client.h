/// \file pd_client.h
/// \brief Class pd_client.
///
/// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#ifndef PD_CLIENT_H
#define PD_CLIENT_H

#include <deque>
#include <string>
#include <vector>
//#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <pd_message.h>
#include <pd_tools.h>


typedef std::deque<pd_message> pd_message_deque;
typedef std::function<void(const std::string& msg, const std::vector<std::string>& args)> pd_on_message_func;


/// pd_client - "Packet Data Client"
class PD_DECL_EXPORT pd_client {
public:
    /// Constructor
	pd_client();

    /// Destructor
    ~pd_client();

	/// \brief Creates a connection to the server.
    /// Runs the client loop in a separate thread.
	/// \param address - remote host address.
	/// \param port - server port.
	void connect(const std::string& address, const std::string& port);

	/// Closes the connection.
	void close();

	/// \brief Logging into the server.
	/// \param user - user name.
	/// \param paswd - user password.
	void login(const std::string& user, const std::string& paswd);

	/// \brief Logout from the server.
	void logout();

	/// \brief Adds a new event.
	/// \param priority - event priority.
	/// \param info - information about the event.
	void add_new_event(int priority, const std::string& info);

	/// \brief Get all events.
	/// Events are received by on_message callback.
	void get_all_events();

	/// \brief Get last event.
	/// The event is received by on_message callback.
	void get_last_event();

	/// \brief Direct transmission of messages to the server.
	/// \param msg - pd_message.
	void write(pd_message &msg);

	/// \brief Sets on_message handler.
	/// It is used to receive messages from the server.
	/// \param handler - function.
	void set_on_message(pd_on_message_func handler);

	/// \brief Returns whether the client is connected.
	/// \return true if connected.
	bool connected();

	/// \brief Returns whether the client is logged in.
	/// \return true if logged in.
	bool loggedin();


private:
	/// Create connection.
	void do_connect();

	/// Reads message header.
	void do_read_header();

	/// Reads message body.
	void do_read_body();

	/// Writes message.
	void do_write();

private:
    std::string address_;                       ///< Server address
    std::string port_;                          ///< Server port
    boost::asio::io_service io_service_;        ///< TCP asynchronous service.
    boost::asio::ip::tcp::socket socket_;       ///< TCP socket.
    pd_message read_msg_;                       ///< Current reading message.
    pd_message_deque write_msgs_;               ///< Message queue.
    std::string user_;                          ///< User name
    std::string password_;                      ///< User password
    pd_on_message_func on_message = nullptr;    ///< Incoming message handler
    bool running = false;                       ///< Running indicator
    boost::thread bth;                          ///< Client thread
};

#endif /* PD_CLIENT_H */
