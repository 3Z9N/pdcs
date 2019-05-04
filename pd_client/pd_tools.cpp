// pd_tools.cpp
// Several tool functions
// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#include <boost/algorithm/string.hpp>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <pd_tools.h>

using namespace CryptoPP;

// Special variable for debugging with DBG macro
// true - enable debug info
// false - disable debug info (default)
bool pd_debug_info = false;

// Encryption of the string with the AES ECB 128 algorithm.
// dst - destination encrypted string.
// src - source string.
// key - key string.
bool AES_encrypt(std::string& dst, const std::string& src,
		const std::string& skey) {
	// set AES key
	SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    for (unsigned int i = 0; i < AES::DEFAULT_KEYLENGTH; i++) {
		if (i < skey.size())
            key[i] = static_cast<byte>(skey[i]);
		else
            key[i] = '*'; // fill with '*'
	}
	// encrypt
	try {
		ECB_Mode<AES>::Encryption e;
		e.SetKey(key, AES::DEFAULT_KEYLENGTH);
		//  The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
        StringSource ss1(src, true,
                new StreamTransformationFilter(e, new StringSink(dst)));
	} catch (CryptoPP::Exception& e) {
        DBG("%s", e.what());
		return false;
	}
	return true;
}

// Decryption of the string with the AES ECB 128 algorithm.
// dst - destination decrypted string.
// src - source string.
// key - key string.
bool AES_decrypt(std::string& dst, const std::string& src,
		const std::string& skey) {
	using namespace std;
	// set AES key
	SecByteBlock key(AES::DEFAULT_KEYLENGTH);
	for (unsigned int i = 0; i < AES::DEFAULT_KEYLENGTH; i++) {
		if (i < skey.size())
            key[i] = static_cast<byte>(skey[i]);
		else
			key[i] = '*'; // fill with *
	}
	// decrypt
	try {
		ECB_Mode<AES>::Decryption d;
		// ECB Mode does not use an IV
		d.SetKey(key, AES::DEFAULT_KEYLENGTH);
		//  The StreamTransformationFilter removes
		//  padding as required.
        StringSource ss3(src, true,
                new StreamTransformationFilter(d, new StringSink(dst)));
	} catch (CryptoPP::Exception& e) {
        DBG("%s", e.what());
		return false;
	}
	return true;
}

// Splits string with delimiter.
// str - splited string.
// delimiter - delimiter string.
std::vector<std::string> split_string(const std::string &str,
		const std::string &delimiter) {
	std::vector<std::string> fields;
	boost::split(fields, str, boost::is_any_of(delimiter));
	return fields;
}

