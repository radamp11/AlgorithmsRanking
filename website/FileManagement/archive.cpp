#include "archive.h"
#include <string>
#include <fstream>
#include <algorithm>
#include "lib/ziplib/Source/ZipLib/ZipFile.h"
#include "exception.h"
//#define CORRECT_NUMBER_OF_FILES 120

using namespace std;

Archive::Archive( string archive_name ) : archive_name_(archive_name){
  const int CEC2017_ENTRIES = 120, CEC2013_ENTRIES = 84;
  ZipArchive::Ptr archive = ZipFile::Open( archive_name );
  number_of_entries_ = archive->GetEntriesCount();
  
  for( int i = 0; i < number_of_entries_; ++i ){  
    auto entry = archive->GetEntry(i);
    entry_name_.push_back( entry->GetFullName() );
  }

  if( number_of_entries_ == CEC2017_ENTRIES )
    cec_year = 2017;
  else if( number_of_entries_ == CEC2013_ENTRIES )
    cec_year = 2013;
  sortEntries();
}

vector<string> Archive::checkDimensions(){
  vector<string> dimensions;
  bool checking_dimensions = true;
  unique_ptr<string[]> separated_name;

  separated_name = separateFullName( entry_name_[0] );
  dimensions.push_back( separated_name[2] );   //under separated_name[2] there is dimension
  for( int i = 1; i < number_of_entries_ && checking_dimensions; ++i ){
    separated_name = separateFullName( entry_name_[i] );
    if( dimensions.front() != separated_name[2] ){
      dimensions.push_back( separated_name[2] );
    }
    else{
      checking_dimensions = false;
    }
  }
  return dimensions;
}

unique_ptr<string[]> Archive::separateFullName( string full_name ){
  unique_ptr<string[]> separated_name{ new string[3] };
  size_t position_first, position_last, position_dot;

  position_last = full_name.find_last_of( '_' );
  position_first = full_name.find_last_of( '_', position_last-1 );
  position_dot = full_name.find_last_of( '.' );

  separated_name[0] = full_name.substr( 0, position_first );
  separated_name[1] = full_name.substr( position_first + 1, position_last - position_first - 1 );   
  separated_name[2] = full_name.substr( position_last + 1, position_dot - position_last - 1 );

  return separated_name;
}

string Archive::findDelimiter(){
  const int BUFFER_SIZE = 50;
  fstream file;
  char ch_str[ BUFFER_SIZE ];
  int first = -1, last = -1;
  bool found_first = false, found_last = false;

  extract( 0 );
  file.open( entry_name_[0], fstream::in );
  file.get( ch_str, BUFFER_SIZE - 1 );
  for( int i = 0; i < BUFFER_SIZE - 1 && !found_first; ++i ){
      if( ( ch_str[i] < '0' || ch_str[i] > '9' ) && ch_str[i] != '.' 
      && ch_str[i] != 'e' && ch_str[i] != 'E' && ch_str[i] != '+' && ch_str[i] != '-' ){
          first = i;
          found_first = true;
      }
  }
  for( int i = first; i < BUFFER_SIZE - 1 && !found_last; ++i ){
      if( ch_str[i] >= '0' && int(ch_str[i]) <= '9' ){
        last = i;
        found_last = true;
      }
  }
  if( last == -1 ){
    throw CppException( "W pliku " + entry_name_[0] + " nie odnaleziono delimitera" );
  }

  string str( ch_str );
  remove( entry_name_[0].c_str() );
  return str.substr(first, last-first);
}

void Archive::sortEntries(){
  unique_ptr<string[]> separated_name;
  unique_ptr<string[]> separated_name2;
  int num = number_of_entries_;

  //bubble sort
  do{
    for( int i = 0; i < number_of_entries_ - 1; ++i ){
    separated_name = separateFullName( entry_name_[i] );
    separated_name2 = separateFullName( entry_name_[i+1] );

      if( stoi( separated_name[1]) > stoi( separated_name2[1] ))
        swap( entry_name_[i], entry_name_[i+1] );
      else if( stoi( separated_name[1]) == stoi( separated_name2[1] ))
        if( stoi( separated_name[2]) > stoi( separated_name2[2] ))
          swap( entry_name_[i], entry_name_[i+1] );
    }
    --num;
  } while ( num-1 );
  
}

void Archive::readCSVData( unique_ptr< vector<string> > &vec, int entry_num, string delimiter ){
  fstream file;
  const int ERROR_LIMIT = 1200;
  string temp_str, value;
  int position;

  file.open( entry_name_[entry_num], fstream::in );

  for( int j = 0; file.good(); ++j ){
    getline( file, temp_str );

    if( temp_str.length() > ERROR_LIMIT )
      throw CppException( "blad w zapisie danych dla pliku " + entry_name_[entry_num] );

    while( temp_str.length() > 0 ){
      position = temp_str.find( delimiter );
      if( position == string::npos )
        position = temp_str.length();
      value = temp_str.substr( 0, position );

      if( value.length() != 0 )
        vec->push_back( value );
      temp_str.erase( 0, position+delimiter.length() );
    }
  }
  file.close();
}

bool Archive::validate(){
  unique_ptr<string[]> separated_name;
  int num_of_fun = 0, num_of_dim = 0;
  bool counting_fun = true;

  separated_name = separateFullName( entry_name_[number_of_entries_ - 1] );
  num_of_fun = stoi( separated_name[1] );
  num_of_dim = checkDimensions().size();

  if( num_of_dim * num_of_fun != number_of_entries_ )
    return false;
  return true;
}

void Archive::extractAll(){
  for( int i = 0; i < number_of_entries_; ++i ){
    ZipFile::ExtractFile( archive_name_, entry_name_[i], entry_name_[i] );
  }
}

void Archive::removeAll(){
  for( int i = 0; i < number_of_entries_; ++i ){
    remove( entry_name_[i].c_str() );
  }
}
void Archive::extract( int entry_idx ){
  if( entry_idx < number_of_entries_ )
    ZipFile::ExtractFile( archive_name_, entry_name_[entry_idx], entry_name_[entry_idx] );
}

int Archive::getNumberOfEntries(){
  return number_of_entries_;
}

string Archive::getEntry( int entry_num ){
  return entry_name_[entry_num];
}

int Archive::getYear(){
  return cec_year;
}