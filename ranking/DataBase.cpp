#include "DataBase.hpp"

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
int DataBase::createTables() {

	std::string algorithms = "CREATE TABLE IF NOT EXISTS ALGORITHMS("
		"ID			INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME		TEXT NOT NULL );";

	std::string testFunctions = "CREATE TABLE IF NOT EXISTS FUNCTIONS("
		"ID			INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME		TEXT NOT NULL );";

	std::string outcome = "CREATE TABLE IF NOT EXISTS OUTCOME("
		"ID			INTEGER PRIMARY KEY AUTOINCREMENT, "
		"ID_ALGORITHM	INTEGER,"
		"ID_FUNCTION	INTEGER,"
		"DIMENSION	INTEGER,"
		"BEST		DOUBLE(10,6),"
		"WORST		DOUBLE(10,6),"
		"MEDIAN		DOUBLE(10,6),"
		"MEAN		DOUBLE(10,6),"
		"STD		DOUBLE(10,6),"
		"FOREIGN KEY(ID_ALGORITHM) REFERENCES ALGORITHMS(ID),"
		"FOREIGN KEY(ID_FUNCTION) REFERENCES FUNCTIONS(ID)"
		");";

	try {
		int exit = 0;
		exit = sqlite3_open(dir, &DB);

		char* messaggeError;
		exit = sqlite3_exec(DB, algorithms.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Algorithms Table" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table Algorithms created Successfully" << std::endl;

		exit = sqlite3_exec(DB, testFunctions.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Functions Table" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table Functions created Successfully" << std::endl;

		exit = sqlite3_exec(DB, outcome.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Outcome Table" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table Outcome created Successfully" << std::endl;

		sqlite3_close(DB);
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}

	return 0;
}


void DataBase::DBinit() {

	createDB();
	createTables();

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
