#include "maintest.cc"
#define BOOST_TEST_MODULE MyTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( my_test )
{
    BOOST_CHECK( fun() != 1 );
}