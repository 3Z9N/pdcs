#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PdTest
#include <boost/test/unit_test.hpp>

#include "pd_message.h"
#include "pd_sqlite.h"
#include "pd_tools.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>

// sqlite creation test
bool database_open() {
    // Remove if exists
    pd_sqlite db;
    if (boost::filesystem::exists(db.file_path())) {
        if(!boost::filesystem::remove(db.file_path()))
            return false;
    }
    return db.open();
}

// sqlite insert test
bool database_insert() {
    pd_sqlite db;
    char* err_str;
    long int timestamp = std::time(nullptr) - 100; // current timestamp minus 100 entries
    char buf[1024];
    // open database
    if(!db.open()) return false;
    // insert 100 sample entries
    //std::cout << "insert n sample entries..." << std::endl;
    for(int i=1; i<=100; i++) {
        err_str = nullptr;
        sprintf(buf,
                "INSERT INTO events(timestamp,priority,info) VALUES(%ld,'%d','%s%d');",
                timestamp, 1, "This is a sample event No. ", i);
        db.exec(buf, nullptr, nullptr, &err_str);
        if(err_str) {
            db.close();
            return false;
        }
        //std::cout << "Creating entry No. " << i << std::endl;
        timestamp++;
    }
    db.close();
    return true;
}



//-------------------------------------
// sqlite testing
//-------------------------------------
BOOST_AUTO_TEST_SUITE( sqlite )
BOOST_AUTO_TEST_CASE( open )
{
    BOOST_CHECK( database_open() );
}
BOOST_AUTO_TEST_CASE( insert )
{
    BOOST_CHECK( database_insert() );
}
BOOST_AUTO_TEST_SUITE_END()

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
