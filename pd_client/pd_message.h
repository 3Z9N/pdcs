/// \file pd_message.h
/// \brief Class pd_message
///
/// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#ifndef PD_MESSAGE_H
#define PD_MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <pd_tools.h>


/// Class pd_message - Custom message container.
class PD_DECL_EXPORT pd_message {
public:
	enum {
		header_length = 4			///< Header size
	};
	enum {
		max_body_length = 512		///< Maximum body length.
	};

	/// Default constructor
	pd_message();

	/// \brief Constructor
	/// \param str - pointer tomessage string
	pd_message(const char* str);

	/// \brief Constructor
	/// \param str - message string
	pd_message(const std::string& str);

	/// \brief Returns the entire message content
	/// \return constant pointer to data
	const char* data() const;

	/// \brief Returns the entire message content
	/// \return pointer to data
    char* data();

	/// \brief Returns the length of the entire message content
	/// \return length of data
	std::size_t length() const;

	/// \brief Returns the message body content
	/// \return constant pointer to body
	const char* body() const;

	/// \brief Returns the message body content
	/// \return pointer to body
    char* body();

	/// \brief Returns the length of the message body content
	/// \return length of body
	std::size_t body_length() const;

	/// \brief Sets the length of the message body content
	/// \param length - body length
	void body_length(std::size_t length);

	/// \brief Decodes message header
	bool decode_header();

	/// \brief Encrypts the message with the given key
	/// \param key - encryption key
	/// \return true if the encryption was successful
	bool encrypt(const std::string& key);

	/// \brief Decrypts the message with the given key
	/// \param key - encryption key
	/// \return true if the decryption was successful
	bool decrypt(const std::string& key);

	/// \brief Returns the message body content as string
	/// \return standard string
	std::string str();

private:
	char data_[header_length + max_body_length + 1];	///< Message container.
	std::size_t body_length_;							///< Body length.
};

#endif // PD_MESSAGE_H
