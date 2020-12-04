#ifndef _DATABASE_HPP
#define _DATABASE_HPP

#include <iostream>
#include "../lib/sqlite/sqlite3.h"


class DataBase
{
public:
	DataBase() : dir("ranking_database.db") {}
	DataBase(const DataBase& database) = delete;

	void DBinit();
	int passSqlQuery(const std::string& query);
	int selectData(const std::string& selectQuery);
	const char* getDir() { return dir; }

	~DataBase() { delete DB; }

private:
	sqlite3* DB;
	const char* dir;


	int createDB();
	int createTables();
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);

};


#endif // !DATABASE_HPP