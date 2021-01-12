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
#include "exception.h"

using namespace std;

//TODO usun warningi (-w) w sconstruct

vector<int> generateResults( string archive_name ){
  const int ONE_ROW_ENTRIES = 51;
  fstream result_file, temp_file;

  if( FILE *test = fopen( archive_name.c_str(), "r" ))
    fclose(test);
  else 
    throw CppException( "archiwum " + archive_name + " nie istnieje" );

  Archive archive( archive_name );
  if( !archive.validate() )
    throw CppException( "archiwum " + archive_name + " nie zawiera odpowiednich plikow / nie przeszlo walidacji" );


  string result_file_name;
  int number_of_values, in_one_row_entries;
  vector<int> dimensions = archive.checkDimensions();
  
  if( archive.getYear() == 2017 )     //na podstawie ilosci dimension zakladamy czy CEC2017 bedzie oblsugiwany czy jednak 2013
  	number_of_values = 714;
  else if ( archive.getYear() == 2013 )
  	number_of_values = 561;
  else
    throw CppException( "Pliki spakowane w archiwum " + archive_name + " nie spelniaja wymogow CEC2017 ani CEC2013" );
  
  unique_ptr< Statistics > stats { new Statistics() };
  unique_ptr< vector<string> > csv_strings { new vector<string>() };

  archive.extractAll();
  for( int for_dim = 0; for_dim < dimensions.size(); ++for_dim ){
    result_file_name = "static/result_files/results" + to_string( dimensions[for_dim] ) + ".txt";
    result_file.open( result_file_name, fstream::out );

    for( int i = 0 + for_dim; i < archive.getNumberOfEntries(); i += dimensions.size() ){
      archive.getFile( i ).readCSVData( csv_strings );

      if( csv_strings->size() != number_of_values ){
        archive.removeAll();
        result_file.close();
        throw CppException( "Wartosci w pliku " + archive.getEntryName( i ) + " sa nieprawidlowe" );
      }

      for( int j = number_of_values - ONE_ROW_ENTRIES; j < csv_strings->size(); ++j )  //branie pod uwage tylko 1.0*MaxFES
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
  
  return dimensions;
}




vector<double> calculateGraph( string archive_name, int benchmark_number, int dimension, int option ){  //trzeba dorobic chyba ze od razu z pliku (podaje sie nazwe pliku aby)
  const int NUMBER_OF_GENERATIONS = 51; //51 * 14 = 714
  const int CONVERGENCE_GRAPH = 0, BOX_PLOT = 1;
  vector<double> calculated_values;

  if( FILE *test = fopen( archive_name.c_str(), "r" ))
        fclose(test);
  else 
        throw CppException( "archiwum " + archive_name + " nie istnieje" );

  int max_fes_types;
  Archive archive( archive_name );
  if( archive.getYear() == 2017 )
    max_fes_types = 14;
  else if ( archive.getYear() == 2013 )
    max_fes_types = 11;
  else
    throw CppException( "Pliki spakowane w archiwum " + archive_name + " nie spelniaja wymogow CEC2017 ani CEC2013" );


  vector<int> dimensions = archive.checkDimensions();
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

  archive.extract( entry_num );
  archive.getFile( entry_num ).readCSVData( csv_strings );

  if( option == BOX_PLOT ){
    for( int i = 0; i < csv_strings->size(); ++i )
      calculated_values.push_back( stod( csv_strings->at(i) ) );
  }

  else if( option == CONVERGENCE_GRAPH ){

    if( csv_strings->size() != NUMBER_OF_GENERATIONS * max_fes_types )
      throw CppException( "plik " + archive.getEntryName( entry_num ) + " jest bledny" );

    for( int i = 0; i < max_fes_types; ++i ){
      for( int j = 0; j < NUMBER_OF_GENERATIONS; ++j )
        mean_val += stod( csv_strings->at( j + i * 51 ));

      mean_val = mean_val / NUMBER_OF_GENERATIONS;
      calculated_values.push_back( mean_val );
    }
  }
  remove( archive.getEntryName( entry_num ).c_str() );
  return calculated_values;
}

int main(){
  return 0;
}

using namespace boost;
using namespace boost::python;

void translate(CppException const& ex)
{
    PyErr_SetString(PyExc_RuntimeError, ex.getPythonInfo().c_str() );
}
BOOST_PYTHON_MODULE(szkielet)
{
    register_exception_translator<CppException>(&translate);
    class_<std::vector<double> >("DoubleVec")
        .def(vector_indexing_suite<std::vector<double> >());
    class_<std::vector<int> >("IntVec")
        .def(vector_indexing_suite<std::vector<int> >());
    def("calculateGraph", calculateGraph);
    def("generateResults", generateResults);
    
}

//  for(int i = 0; i < calculated_values.size(); ++i )
//    cout<<calculated_values[i]<<"\n";sf
