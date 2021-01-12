#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string>
#include <vector>
#include <memory>

class Archive{
public:
    Archive( std::string archive_name );
    void extractAll();
    void removeAll();
    void extract( int num_of_entry );
    void sortEntries();
    bool validate();
    std::vector<std::string> checkDimensions();
    std::unique_ptr<std::string[]> separateFullName( std::string full_name );
    void readCSVData( std::unique_ptr<std::vector<std::string>> &vec, int entry_num, std::string delimiter );
    std::string findDelimiter();
    int getNumberOfEntries();
    std::string getEntry( int entry_num );
    int getYear();

private:
    int cec_year;
    int number_of_entries_;
    std::string archive_name_;
    std::vector<std::string> entry_name_; 
};

#endif 