#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string>
#include <vector>
#include "file.h"

class Archive{
public:
    Archive( std::string archive_name );
    void extractAll();
    void removeAll();
    void extract( int num_of_entry );
    void sortEntries();
    bool validate();
    std::vector<int> checkDimensions();

    int getNumberOfEntries();
    std::string getEntryName( int entry_num );
    int getYear();
    File getFile( int file_num );

private:
    int cec_year_;
    int number_of_entries_;
    std::string archive_name_;
    std::vector<File> file_;
};

#endif 