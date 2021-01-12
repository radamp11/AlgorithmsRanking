#include "file.h"
#include <string>
#include <fstream>
#include "lib/ziplib/Source/ZipLib/ZipFile.h"
#include "exception.h"

using namespace std;

File::File( std::string file_name ) : file_name_(file_name){
    unique_ptr<string[]> separated_name = separateFullName();

    algorithm_name_ = separated_name[0];
    benchmark_function_ = stoi( separated_name[1] );
    dimension_ = stoi( separated_name[2] );

    
}

unique_ptr<string[]> File::separateFullName(){
  unique_ptr<string[]> separated_name{ new string[3] };
  size_t position_first, position_last, position_dot;

  position_last = file_name_.find_last_of( '_' );
  position_first = file_name_.find_last_of( '_', position_last-1 );
  position_dot = file_name_.find_last_of( '.' );

  separated_name[0] = file_name_.substr( 0, position_first );
  separated_name[1] = file_name_.substr( position_first + 1, position_last - position_first - 1 );   
  separated_name[2] = file_name_.substr( position_last + 1, position_dot - position_last - 1 );

  return separated_name;
}



void File::readCSVData( unique_ptr< vector<string> > &vec ){ //zaklada ze plik jest wypakowany

  if( FILE *test = fopen( file_name_.c_str(), "r" ))
    fclose(test);
  else 
    throw CppException( "Plik " + file_name_ + " nie istnieje / nie zostal wypakowany" );

  fstream file;
  const int ERROR_LIMIT = 1200;
  string temp_str, value;
  int position;

  file.open( file_name_, fstream::in );

  for( int j = 0; file.good(); ++j ){
    getline( file, temp_str );

    if( temp_str.length() > ERROR_LIMIT )
      throw CppException( "blad w zapisie danych dla pliku " + file_name_ );

    while( temp_str.length() > 0 ){
      position = temp_str.find( delimiter_ );
      if( position == string::npos )
        position = temp_str.length();
      value = temp_str.substr( 0, position );

      if( value.length() != 0 )
        vec->push_back( value );
      temp_str.erase( 0, position+delimiter_.length() );
    }
  }
  file.close();
}

void File::findDelimiter(){
  if( FILE *test = fopen( file_name_.c_str(), "r" ))
    fclose(test);
  else 
    throw CppException( "Plik " + file_name_ + " nie istnieje / nie zostal wypakowany" );

  const int BUFFER_SIZE = 50;
  fstream file;
  char ch_str[ BUFFER_SIZE ];
  int first = -1, last = -1;
  bool found_first = false, found_last = false;

  file.open( file_name_, fstream::in );
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
    throw CppException( "W pliku " + file_name_ + " nie odnaleziono delimitera" );
  }

  string str( ch_str );
  delimiter_ = str.substr(first, last-first);
}


string File::getFileName(){
    return file_name_;
}

int File::getFunction(){
    return benchmark_function_;
}

int File::getDimension(){
    return dimension_;
}

std::string File::getDelimiter(){
    return delimiter_;
}

void File::setDelimiter( string del ){
    delimiter_ = del;
}