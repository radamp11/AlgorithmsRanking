#ifndef _DATABASE_HPP
#define _DATABASE_HPP

#include <iostream>
#include <sqlite3.h>


class DataBase
{
public:
	DataBase() : dir("D:\\01 STUDIA\\SEM5\\ZPR\\PROJEKT\\EvolutionaryRanking\\RANKING_DATABASE.db") {}
	DataBase(const DataBase& database) = delete;	// temporarily
	int createDB();
	int createTable();
	int passSqlQuery(const std::string& query);
	int selectData(const std::string& selectQuery);
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	const char* getDir() { return dir; }
	void DBinit();
	~DataBase() { delete DB; }

private:
	sqlite3* DB;
	const char* dir;

};


#endif // !DATABASE_HPP