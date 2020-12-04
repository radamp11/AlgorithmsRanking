#include <iostream>
#include <sqlite3.h>
#include "DataBase.hpp"

int main()
{
    DataBase* database = new DataBase();

    std::string query = "INSERT INTO GRADES (NAME, LNAME, AGE, ADDRESS, GRADE) VALUES ('Adam', 'Lastname', 30, 'Exactly here', 'A+');";

    database->DBinit();
    database->passSqlQuery(query);
    database->selectData("SELECT * FROM GRADES;");

}
