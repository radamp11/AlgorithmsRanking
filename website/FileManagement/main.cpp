#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "lib/ziplib/Source/ZipLib/ZipFile.h"
#include "archive.h"
#include "statistics.h"

//TODO usun warningi (-w) w sconstruct
using namespace std;

void generateResults( string archive_name ){
  const int EXPECTED_NUMBER_OF_ENTRIES = 714;
  fstream result_file, temp_file;
    if( FILE *test = fopen( archive_name.c_str(), "r" )) {
        fclose(test);
  } else {
        cerr << "archiwum nie istnieje";
        exit(1);
  }

  string result_file_name, delimiter;
  Archive archive( archive_name );
  vector<string> dimensions = archive.checkDimensions();
  Statistics* stats = new Statistics();  //TODO errorhandling
  vector<string>* csv_strings = new vector<string>;

  delimiter = archive.findDelimiter();
  archive.extractAll();
  for( int for_dim = 0; for_dim < dimensions.size(); ++for_dim ){
    result_file_name = "results" + dimensions[for_dim] + ".txt";
    result_file.open( result_file_name, fstream::out );

    for( int i = 0 + for_dim; i < archive.getNumberOfEntries(); i += 4 ){
      archive.readCSVData( csv_strings, i, delimiter );

      if( csv_strings->size() != EXPECTED_NUMBER_OF_ENTRIES )
        cout<< csv_strings->size();

      for( int j = 0; j < csv_strings->size(); ++j )
        stats->insertValue( stod( csv_strings->at( j ) ));

      stats->calculateStatistics();

      result_file << stats->getComponents()->best_ << " " << stats->getComponents()->worst_ << " ";
      result_file << stats->getComponents()->median_ << " " << stats->getComponents()->mean_ << " ";
      result_file << stats->getComponents()->std_ <<"\n";

      stats->cleanValues();
      csv_strings->clear();
    }
    result_file.close();
  }
  archive.removeAll();
  delete( csv_strings );
  delete( stats );
}




vector<double> calculateConvergenceGraph( string archive_name, int benchmark_number, int dimension_idx ){  //trzeba dorobic chyba ze od razu z pliku (podaje sie nazwe pliku aby)
  const int NUMBER_OF_GENERATIONS = 51, MAX_FES_TYPES = 14; //51 * 14 = 714
  
  if( FILE *test = fopen( archive_name.c_str(), "r" )) {
        fclose(test);
  } else {
        cerr << "archiwum nie istnieje";
        exit(1);
  }

  Archive archive( archive_name );
  vector<string> dimensions = archive.checkDimensions();
  if( dimension_idx >= dimensions.size() || dimension_idx < 0 )
    exit(1);
  if( benchmark_number > archive.getNumberOfEntries() / dimensions.size() || benchmark_number < 1 )
    exit(1);

  int entry_num = ( benchmark_number-1 ) * dimensions.size() + dimension_idx;


  string delimiter;
  double mean_val = 0;
  vector<double> calculated_mean_values;
  vector<string>* csv_strings = new vector<string>;

  delimiter = archive.findDelimiter();
  archive.extract( entry_num );
  
  archive.readCSVData( csv_strings, entry_num, delimiter );
  if( csv_strings->size() != NUMBER_OF_GENERATIONS * MAX_FES_TYPES )   
    cerr << "niepelny plik csv";
  for( int i = 0; i < MAX_FES_TYPES; ++i ){
    for( int j = 0; j < NUMBER_OF_GENERATIONS; ++j )
      mean_val += stod( csv_strings->at( j + i * 51 ));
    mean_val = mean_val / NUMBER_OF_GENERATIONS;
    calculated_mean_values.push_back( mean_val );
  }
    for(int i = 0; i < calculated_mean_values.size(); ++i )
    cout<<calculated_mean_values[i]<<"\n";

  remove( archive.getEntry( entry_num ).c_str() );
  delete( csv_strings );
  return calculated_mean_values;                     
}

int main(){
  return 0;
}

using namespace boost;
using namespace boost::python;


BOOST_PYTHON_MODULE(szkielet)
{
    class_<std::vector<double> >("DoubleVec")
        .def(vector_indexing_suite<std::vector<double> >());
    //class_<std::vector<string> >("StringVec")
    //    .def(vector_indexing_suite<std::vector<string> >());
    //def("extract", extractArchive);
    def("calculateConvergenceGraph", calculateConvergenceGraph);
    def("generateResults", generateResults);
}

//  for(int i = 0; i < calculated_mean_values.size(); ++i )
//    cout<<calculated_mean_values[i]<<"\n";sf
