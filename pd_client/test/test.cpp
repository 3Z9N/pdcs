#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PdTest
#include <boost/test/unit_test.hpp>

#include "pd_message.h"
#include "pd_tools.h"
#include "pd_client.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>



//-------------------------------------
// tools testing
//-------------------------------------
BOOST_AUTO_TEST_SUITE( tools )
BOOST_AUTO_TEST_CASE( AES_encrypt_decrypt )
{
    std::string text = "This is a sample text";
    std::string key = "Sample_key";
    std::string encrypted;
    std::string decrypted;
    BOOST_CHECK( AES_encrypt(encrypted, text, key) );
    BOOST_CHECK( AES_decrypt(decrypted, encrypted, key) );
    BOOST_CHECK( decrypted == text );
}
BOOST_AUTO_TEST_CASE( split_string_func )
{
    std::string line = "aaa|bbb|ccc|ddd|eee";
    std::vector<std::string> v = split_string(line, "|");
    BOOST_CHECK( v.size() == 5 );
}
BOOST_AUTO_TEST_SUITE_END()

//-------------------------------------
// message testing
//-------------------------------------
BOOST_AUTO_TEST_SUITE( message )
BOOST_AUTO_TEST_CASE( body )
{
    std::string text = "This is a sample text";
    pd_message msg(text);
    BOOST_CHECK( text.length() == msg.body_length() );
    BOOST_CHECK( text == msg.str() );
}
BOOST_AUTO_TEST_CASE( encrypt_decrypt )
{
    std::string text = "This is a sample text";
    pd_message msg(text);
    BOOST_CHECK( msg.encrypt("sample_key") );
    BOOST_CHECK( msg.decrypt("sample_key") );
    BOOST_CHECK( msg.str() == text );
}
BOOST_AUTO_TEST_SUITE_END()

//-------------------------------------
// client testing
//-------------------------------------
BOOST_AUTO_TEST_SUITE( client )
BOOST_AUTO_TEST_CASE( connect )
{
    pd_client cli;
    cli.connect("localhost", "11111");
    BOOST_CHECK( cli.connected() );
    if(!cli.connected())
        std::cout << "Check if the server is running" << std::endl;
    cli.login("admin", "admin");
    BOOST_CHECK( cli.connected() );
}
BOOST_AUTO_TEST_SUITE_END()
