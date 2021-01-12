#include "archive.h"
#include "file.h"
#define BOOST_TEST_MODULE MyTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <vector>

Archive dobry("RB.zip");

BOOST_AUTO_TEST_CASE( my_test )
{
    //Archive bledny("bledny.zip");
    //BOOST_CHECK( dobry.validate() != false );
    //BOOST_CHECK( bledny.validate() == false );
}

//./boostTest/tests --log_level=test_suite
