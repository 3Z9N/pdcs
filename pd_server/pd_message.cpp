// pd_message.cpp
// Class pd_message
// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#include <pd_message.h>
#include <pd_tools.h>

// Default constructor
pd_message::pd_message() : body_length_(0) {
}

// Constructor
// str - pointer tomessage string
pd_message::pd_message(const char* cstr) {
    body_length(std::strlen(cstr));
    std::sprintf(data_, "%4ld", body_length_);
    std::memcpy(body(), cstr, body_length_);
}

// Constructor
// str - message string
pd_message::pd_message(const std::string& str) {
    body_length(str.size());
    std::sprintf(data_, "%4ld", body_length_);
    std::memcpy(body(), str.c_str(), body_length_);
}

// Returns the entire message content
// return constant pointer to data
const char* pd_message::data() const {
    return data_;
}

// Returns the entire message content
// return pointer to data
char* pd_message::data() {
    return data_;
}

// Returns the length of the entire message content
// return length of data
std::size_t pd_message::length() const {
    return header_length + body_length_;
}

// Returns the message body content
// constant pointer to body
const char* pd_message::body() const {
    return data_ + header_length;
}

// Returns the message body content
// return pointer to body
char* pd_message::body() {
    return data_ + header_length;
}

// Returns the length of the message body content
// length of body
std::size_t pd_message::body_length() const {
    return body_length_;
}

// Sets the length of the message body content
// length - body length
void pd_message::body_length(std::size_t length) {
    body_length_ = length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

// Decodes message header
bool pd_message::decode_header() {
    std::string s(data_, 4);
    body_length_ = std::stoul(s);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

// Encrypts the message with the given key
// key - encryption key
// return true if the encryption was successful
bool pd_message::encrypt(const std::string& key) {
    std::string encstr;
    if(AES_encrypt(encstr, std::string(body(), body_length_), key)) {
        *this = pd_message(encstr);
        return true;
    }
    return false;
}

// Decrypts the message with the given key
// key - encryption key
// return true if the decryption was successful
bool pd_message::decrypt(const std::string& key) {
    std::string decstr;
    if(AES_decrypt(decstr, std::string(body(), body_length_), key)) {
        *this = pd_message(decstr);
        return true;
    }
    return false;
}

// Returns the message body content as string
// return standard string
std::string pd_message::str() {
    return std::string(body(), body_length_);
}

