#define BOOST_TEST_MODULE MyTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <vector>
#include <memory>
#include "../archive.h"
#include "../file.h"
#include "../exception.h"

using namespace std;

Archive dobry("RB.zip");
Archive dobry2("DES.zip");
Archive dobry2013("2013.zip");
Archive bledny("bledny.zip");
File file_dobry("dobry_1_10.txt");
File file_bledny("bledny_2_100.txt");

BOOST_AUTO_TEST_SUITE( Archiwum )
BOOST_AUTO_TEST_CASE( validationTest )
{
	BOOST_CHECK( dobry.validate() != false );
	BOOST_CHECK( bledny.validate() == false );
	BOOST_CHECK_THROW( Archive nie_istnieje(".zip"), CppException );
}
BOOST_AUTO_TEST_CASE( dimensionCheckTest )
{
	BOOST_CHECK_EQUAL( dobry.checkDimensions().size(), 4 );
	BOOST_CHECK_EQUAL( dobry2.checkDimensions().size(), 4 );
	BOOST_CHECK_EQUAL( dobry2013.checkDimensions().size(), 3 );
	vector<int> dim = dobry.checkDimensions();
	for( int i = 0; i < 3; ++i ){
		BOOST_CHECK( dim[i] < dim[i+1] );
	}
	BOOST_CHECK_EQUAL( dim[0], 10 );
	BOOST_CHECK_EQUAL( dim[1], 30 );
	BOOST_CHECK_EQUAL( dim[2], 50 );
	BOOST_CHECK_EQUAL( dim[3], 100 );
	
	dim = dobry2013.checkDimensions();
	BOOST_CHECK_EQUAL( dim[0], 10 );
	BOOST_CHECK_EQUAL( dim[1], 30 );
	BOOST_CHECK_EQUAL( dim[2], 50 );
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( pliki )
BOOST_AUTO_TEST_CASE( delimiterTest )
{
	file_dobry.findDelimiter();
	string delimiter = file_dobry.getDelimiter();
	BOOST_CHECK_EQUAL( delimiter, "," );
	
	file_bledny.setDelimiter(" ");
	BOOST_CHECK_EQUAL( file_bledny.getDelimiter(), " " );
	
	BOOST_CHECK_THROW( file_bledny.findDelimiter(), CppException );
}
BOOST_AUTO_TEST_CASE( separatingTest )
{
	std::unique_ptr<std::string[]> separate_string;
	separate_string = file_dobry.separateFullName();
	BOOST_CHECK_EQUAL( separate_string[0], "dobry" );
	BOOST_CHECK_EQUAL( separate_string[1], "1" );
	BOOST_CHECK_EQUAL( separate_string[2], "10" );
	
	separate_string = file_bledny.separateFullName();
	BOOST_CHECK_EQUAL( separate_string[0], "bledny" );
	BOOST_CHECK_EQUAL( separate_string[1], "2" );
	BOOST_CHECK_EQUAL( separate_string[2], "100" );
	
}
BOOST_AUTO_TEST_CASE( readCSVTest )
{
	unique_ptr< vector<string> > csv_strings { new vector<string>() };
	file_dobry.readCSVData( csv_strings );
	BOOST_CHECK( csv_strings->size() == 714 );
	BOOST_CHECK_EQUAL( csv_strings->at(0), "221820342.269274" );
	BOOST_CHECK_EQUAL( csv_strings->at(51), "31548023.1688067" );
	BOOST_CHECK_EQUAL( csv_strings->at(713), "1.17919199738026e-07" );
	
	File nie_istnieje( "brak.txt" );
	BOOST_CHECK_THROW( nie_istnieje.readCSVData ( csv_strings ), CppException );
	
	File krotki_csv( "krotki.txt" );
	krotki_csv.findDelimiter();
	krotki_csv.readCSVData( csv_strings );
	BOOST_CHECK( csv_strings->at(0) != "22182" );
	
	csv_strings->clear();
	krotki_csv.readCSVData( csv_strings );
	BOOST_CHECK_EQUAL( csv_strings->at(0), "22182" );
	
}
BOOST_AUTO_TEST_SUITE_END()
