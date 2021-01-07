#include <iostream>
#include "../lib/ziplib/Source/ZipLib/ZipFile.h"
#include "../lib/ziplib/Source/ZipLib/streams/memstream.h"
#include "../lib/ziplib/Source/ZipLib/methods/Bzip2Method.h"

int main(int argc, char* argv[]){
  if( argc != 2 ){
  	std::cout<<"Nie podano nazwy archiwum";
  	return 1;
  }

  const char* zipFilename = argv[1];
  ZipFile::ExtractFile(zipFilename, "file.txt", "extracted.txt");


  return 0;
}