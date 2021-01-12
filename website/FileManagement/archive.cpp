#include "archive.h"
#include <string>
#include <fstream>
#include <algorithm>
#include "lib/ziplib/Source/ZipLib/ZipFile.h"
#include "exception.h"
#include "file.h"
//#define CORRECT_NUMBER_OF_FILES 120

using namespace std;

Archive::Archive( string archive_name ) : archive_name_(archive_name){
  const int CEC2017_ENTRIES = 120, CEC2013_ENTRIES = 84;
  ZipArchive::Ptr archive = ZipFile::Open( archive_name );
  number_of_entries_ = archive->GetEntriesCount();
  
  for( int i = 0; i < number_of_entries_; ++i ){  
    auto entry = archive->GetEntry(i);
    file_.push_back( entry->GetFullName() );
  }

  if( number_of_entries_ == CEC2017_ENTRIES )
    cec_year_ = 2017;
  else if( number_of_entries_ == CEC2013_ENTRIES )
    cec_year_ = 2013;

  sortEntries();

  extract( 0 );
  
  file_[0].findDelimiter();                   //zakladamy ze wszystkie pliki w archiwum maja taki sam delimiter;
  string delimiter = file_[0].getDelimiter();
  for( int i = 1; i < number_of_entries_; ++i )
    file_[i].setDelimiter( delimiter );

  remove( file_[0].getFileName().c_str() );
}

vector<int> Archive::checkDimensions(){
  vector<int> dimensions_vec;
  bool checking_dimensions = true;
  int dimension;

  dimension = file_[0].getDimension();
  dimensions_vec.push_back( dimension );
  for( int i = 1; i < number_of_entries_ && checking_dimensions; ++i ){
    dimension = file_[i].getDimension();
    if( dimensions_vec.front() != dimension ){
      dimensions_vec.push_back( dimension );
    }
    else{
      checking_dimensions = false;
    }
  }
  return dimensions_vec;
}

void Archive::sortEntries(){
  int num = number_of_entries_;

  //bubble sort
  do{
    for( int i = 0; i < number_of_entries_ - 1; ++i ){

      if( file_[i].getFunction() > file_[i+1].getFunction() )
        swap( file_[i], file_[i+1] );
      else if( file_[i].getFunction() == file_[i+1].getFunction() )
        if( file_[i].getDimension() > file_[i+1].getDimension() )
          swap( file_[i], file_[i+1] );
    }
    --num;
  } while ( num-1 );
  
}

bool Archive::validate(){
  int num_of_fun = 0, num_of_dim = 0;
  bool counting_fun = true;

  num_of_fun = file_[number_of_entries_ - 1].getFunction();
  num_of_dim = checkDimensions().size();

  if( num_of_dim * num_of_fun != number_of_entries_ )
    return false;
  return true;
}

void Archive::extractAll(){
  for( int i = 0; i < number_of_entries_; ++i ){
    ZipFile::ExtractFile( archive_name_, file_[i].getFileName(), file_[i].getFileName() );
  }
}

void Archive::removeAll(){
  for( int i = 0; i < number_of_entries_; ++i ){
    remove( file_[i].getFileName().c_str() );
  }
}
void Archive::extract( int entry_idx ){
  if( entry_idx < number_of_entries_ )
    ZipFile::ExtractFile( archive_name_, file_[entry_idx].getFileName(), file_[entry_idx].getFileName() );
}

int Archive::getNumberOfEntries(){
  return number_of_entries_;
}

string Archive::getEntryName( int entry_num ){
  return file_[entry_num].getFileName();
}

int Archive::getYear(){
  return cec_year_;
}

File Archive::getFile( int file_num ){
  return file_[file_num];
}