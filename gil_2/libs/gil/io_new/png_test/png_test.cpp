// png_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost::gil;

typedef png_tag tag_t;

BOOST_AUTO_TEST_CASE( read_image_info_using_string )
{
    std::string filename( "..\\test_images\\png\\wikipedia\\test.png" );

    {
        image_read_info< png_tag > info = boost::gil::read_image_info( filename
                                                                     , tag_t() );
        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        image_read_info< tag_t > info = boost::gil::read_image_info( in
                                                                   , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    {
        string filename( "..\\test\\png\\write_view_test.jpg" );

        gray8_image_t img;
        write_view( filename, view( img ), tag_t() );
    }
}