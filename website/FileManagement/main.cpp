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

vector<int> generateResults( string archive_name ){
  const int EXPECTED_NUMBER_OF_ENTRIES = 714;
  fstream result_file, temp_file;
  vector<int> int_dimensions;

    if( FILE *test = fopen( archive_name.c_str(), "r" )) {
        fclose(test);
  } else {
        cerr << "archiwum nie istnieje";
        return int_dimensions;
  }



  Archive archive( archive_name );
  if( !archive.validate() ){
    cerr << "archiwum nie zawiera odpowiednich plikow";
    return int_dimensions;
  }


  string result_file_name, delimiter;
  vector<string> dimensions = archive.checkDimensions();
  unique_ptr< Statistics > stats { new Statistics() };  //TODO errorhandling
  unique_ptr< vector<string> > csv_strings { new vector<string>() };

  delimiter = archive.findDelimiter();
  archive.extractAll();
  for( int for_dim = 0; for_dim < dimensions.size(); ++for_dim ){
    result_file_name = "results" + dimensions[for_dim] + ".txt";
    result_file.open( result_file_name, fstream::out );

    for( int i = 0 + for_dim; i < archive.getNumberOfEntries(); i += 4 ){
      archive.readCSVData( csv_strings, i, delimiter );

      if( csv_strings->size() != EXPECTED_NUMBER_OF_ENTRIES ){
        archive.removeAll();
        cerr << "pliki zawarte w archiwum sa bledne";
        return int_dimensions;
      }

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

  for(int i = 0; i < dimensions.size(); ++i ){
    int_dimensions.push_back( stoi( dimensions[i] ));
  }
  
  return int_dimensions;
}




vector<double> calculateGraph( string archive_name, int benchmark_number, string dimension, int option ){  //trzeba dorobic chyba ze od razu z pliku (podaje sie nazwe pliku aby)
  const int NUMBER_OF_GENERATIONS = 51, MAX_FES_TYPES = 14; //51 * 14 = 714
  const int CONVERGENCE_GRAPH = 0, BOX_PLOT = 1;
  vector<double> calculated_values;

  if( FILE *test = fopen( archive_name.c_str(), "r" )) {
        fclose(test);
  } else {
        cerr << "archiwum nie istnieje";
        return calculated_values;
  }


  Archive archive( archive_name );
  vector<string> dimensions = archive.checkDimensions();
  int dimension_idx = -1;
  for( int i = 0; i < dimensions.size(); ++i )
    if( dimensions[i] == dimension )
      dimension_idx = i;
  
    if( dimension_idx >= dimensions.size() || dimension_idx < 0 ){
      cerr << " Niewlasciwe dimension, zmieniam wartosc na dozwolona";
      dimension_idx = 0;
    } 
    if( benchmark_number > archive.getNumberOfEntries() / dimensions.size() || benchmark_number < 1 ){
      cerr << " Niewlasciwy benchmark number, zmieniam wartosc na dozwolona";
      benchmark_number = 1;
    }

  int entry_num = ( benchmark_number-1 ) * dimensions.size() + dimension_idx;


  string delimiter;
  double mean_val = 0;
  unique_ptr< vector<string> > csv_strings { new vector<string>() };

  try{
    delimiter = archive.findDelimiter();
  } catch ( string str ){
    cerr << "nie udalo sie znalezc delimitera";
    return calculated_values;
  }

  archive.extract( entry_num );
  archive.readCSVData( csv_strings, entry_num, delimiter );

  if( option == BOX_PLOT ){
    for( int i = 0; i < csv_strings->size(); ++i )
      calculated_values.push_back( stod( csv_strings->at(i) ) );
  }

  else if( option == CONVERGENCE_GRAPH ){
    if( csv_strings->size() != NUMBER_OF_GENERATIONS * MAX_FES_TYPES )   
      cerr << "niepelny plik csv";

    for( int i = 0; i < MAX_FES_TYPES; ++i ){
      for( int j = 0; j < NUMBER_OF_GENERATIONS; ++j )
        mean_val += stod( csv_strings->at( j + i * 51 ));

      mean_val = mean_val / NUMBER_OF_GENERATIONS;
      calculated_values.push_back( mean_val );
    }
  }
  remove( archive.getEntry( entry_num ).c_str() );
  return calculated_values;
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
    class_<std::vector<int> >("IntVec")
        .def(vector_indexing_suite<std::vector<int> >());
    def("calculateGraph", calculateGraph);
    def("generateResults", generateResults);
}

//  for(int i = 0; i < calculated_values.size(); ++i )
//    cout<<calculated_values[i]<<"\n";sf
