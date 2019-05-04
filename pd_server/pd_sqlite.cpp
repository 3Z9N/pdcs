// pd_sqlite.cpp
// Class pd_sqlite - Simple wrapper class for sqlite3 library.
//
// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#include "pd_sqlite.h"
#include <boost/filesystem.hpp>
#include <iostream>

// Database creation query.
static const char *create_query =
		"CREATE TABLE events ("
		"  timestamp INTEGER PRIMARY KEY,"
        "  priority  INTEGER,"
        "  info      TEXT"
		");";

// Constructor.
pd_sqlite::pd_sqlite() {
    db = nullptr;
    fname = "events.db";
    fpath = boost::filesystem::current_path().string() + "/events.db";
}

// Destructor.
pd_sqlite::~pd_sqlite() {
	if (db)
		sqlite3_close(db);
}

// Opens the database connection.
// If the database does not exist, it creates a new one
bool pd_sqlite::open() {
	bool need_create = false;
    if (!boost::filesystem::exists(fpath))
		need_create = true;
    int res = sqlite3_open(fpath.c_str(), &db);
	if (res) {
        return true;
	}
	if (need_create) {
        char *err_str = nullptr;
        sqlite3_exec(db, create_query, nullptr, nullptr, &err_str);
		if (err_str) {
			std::cout << "pd_sqlite::open ERROR: " << err_str << std::endl;
            return false;
		}
	}
    return true;
}

// Closes the database connection.
void pd_sqlite::close() {
	if (db)
		sqlite3_close(db);
    db = nullptr;
}

// Execute SQL query.
// query - SQL query string.
// callback - sqlite3_exec callback function.
// cbk_data - data pointer for the first parameter of the callback function.
// errmsg - string pointer for error information.
void pd_sqlite::exec(const char* query,
		int (*callback)(void*, int, char**, char**), void* cbk_data,
		char** errmsg) {
    sqlite3_exec(db, query, callback, cbk_data, errmsg);
}

// Returns database file name.
const std::string &pd_sqlite::file_name()
{
    return fname;
}

// Returns path to database file.
const std::string &pd_sqlite::file_path()
{
    return fpath;
}

