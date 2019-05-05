/// \file pd_tools.h
/// \brief Several tool functions
///
/// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#ifndef PD_TOOLS_H
#define PD_TOOLS_H

#include <vector>
#include <string>
#include <cstdio>

/// \brief PD_DECL_EXPORT - This macro marks a symbol for shared library export.
/// \brief PD_DECL_IMPORT - This macro marks a symbol for shared library import.
#if defined(_WIN32) || defined(_WIN64)
    #define PD_DECL_EXPORT __declspec(dllexport)
    #define PD_DECL_IMPORT __declspec(dllimport)
#else
    #define PD_DECL_EXPORT
    #define PD_DECL_IMPORT
#endif

/// \brief Special variable for debugging with DBG macro
/// true - enable debug info
/// false - disable debug info (default)
extern bool PD_DECL_EXPORT pd_debug_info;

/// \brief Special macro for debugging
#define DBG(fmt, ...) \
    if(pd_debug_info) std::printf("DBG [%s:%d: %s] " fmt "\n", \
    __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

/// \brief Encryption of the string with the AES ECB 128 algorithm.
/// \param dst - destination encrypted string.
/// \param src - source string.
/// \param key - encryption key.
bool PD_DECL_EXPORT AES_encrypt(std::string& dst, const std::string& src,
		const std::string& key);

/// \brief Decryption of the string with the AES ECB 128 algorithm.
/// \param dst - destination decrypted string.
/// \param src - source string.
/// \param key - encryption key.
bool PD_DECL_EXPORT AES_decrypt(std::string& dst, const std::string& src,
		const std::string& key);

/// \brief Splits string with delimiter.
/// \param str - splited string.
/// \param delimiter - delimiter string.
std::vector<std::string> PD_DECL_EXPORT split_string(const std::string &str,
		const std::string &delimiter);


#endif // PD_TOOLS_H
