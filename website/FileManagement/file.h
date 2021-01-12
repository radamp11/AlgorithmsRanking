#ifndef FILE_H
#define FILE_H

#include <string>

class File{
public:
    std::vector<std::string> checkDimensions();
    std::unique_ptr<std::string[]> separateFullName( std::string full_name );
    void readCSVData( std::unique_ptr<std::vector<std::string>> &vec, int entry_num, std::string delimiter );
    std::string findDelimiter();

private:
    std::string file_name;
    std::string delimiter;
}

#ifndef