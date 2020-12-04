#include <stdio.h>
#include "DataBase.hpp"

extern "C"{

int DataBase::passSqlQuery(const std::string& query) {

	try {
		int exit = 0;
		exit = sqlite3_open(dir, &DB);

		char* messaggeError;
		exit = sqlite3_exec(DB, query.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error in query" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "The output of a query: " << std::endl;

		sqlite3_close(DB);
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}

	return 0;
}

int DataBase::createDB() {

	int exit = 0;

	exit = sqlite3_open(dir, &DB);	// when there is no database, it is being created right here

	sqlite3_close(DB);

	return 0;
}
int DataBase::createTable() {

	std::string sql = "CREATE TABLE IF NOT EXISTS GRADES("
		"ID			INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME		TEXT NOT NULL, "
		"LNAME		TEXT NOT NULL, "
		"AGE		INT NOT NULL, "
		"ADDRESS	VARCHAR(50), "
		"GRADE		VARCHAR(30) );";

	try {
		int exit = 0;
		exit = sqlite3_open(dir, &DB);

		char* messaggeError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table created Successfully" << std::endl;

		sqlite3_close(DB);
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}

	return 0;
}

void DataBase::DBinit() {

	createDB();
	createTable();

}

int DataBase::selectData(const std::string& selectQuery) {

	int exit = sqlite3_open(dir, &DB);

	sqlite3_exec(DB, selectQuery.c_str(), callback, NULL, NULL);

	return 0;
}
int DataBase::callback(void* NotUsed, int argc, char** argv, char** azColName) {
	
	for (int i = 0; i < argc; ++i) {
		std::cout << azColName[i] << ": " << argv[i] << std::endl;
	}

	std::cout << std::endl;

	return 0;
}

}