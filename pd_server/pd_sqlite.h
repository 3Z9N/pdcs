/// \file pd_sqlite.h
/// \brief Class pd_sqlite - Simple wrapper class for sqlite3 library.
///
/// Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)

#ifndef PD_SQLITE_H_
#define PD_SQLITE_H_

#include <sqlite3.h>
#include <string>

/// Class pd_sqlite - Simple wrapper class for sqlite3 library.
class pd_sqlite {
public:
	/// Constructor.
	pd_sqlite();

	/// Destructor.
	virtual ~pd_sqlite();

    /// \brief Opens the database connection.
    /// If the database does not exist, it creates a new one
    /// \return - Returns true if it succeeds
    bool open();

	/// Closes the database connection.
	void close();

	/// \brief Execute SQL query.
	/// \param query - SQL query string.
	/// \param callback - sqlite3_exec callback.
    /// \param data - data pointer for the first parameter of the callback function.
    /// \param errmsg - string pointer to error message.
	void exec(const char* query, int (*callback)(void*, int, char**, char**),
            void* data, char **errmsg);

    /// Returns database file name.
    const std::string& file_name();

    /// Returns path to database file.
    const std::string& file_path();

private:
    sqlite3 *db;            ///< sqlite3 database pointer.
    std::string fname;      ///< database file name
    std::string fpath;      ///< path do database file
};

#endif /* PD_SQLITE_H_ */
