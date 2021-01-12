#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>
#include <memory>

class File{
public:
    File( std::string file_name );
    std::unique_ptr<std::string[]> separateFullName();
    void readCSVData( std::unique_ptr<std::vector<std::string>> &vec );
    void findDelimiter();

    std::string getFileName();
    int getFunction();
    int getDimension();
    std::string getDelimiter();
    void setDelimiter( std::string del );

private:
    std::string file_name_;
    std::string algorithm_name_;
    int benchmark_function_;
    int dimension_;
    std::string delimiter_;
};

#endif