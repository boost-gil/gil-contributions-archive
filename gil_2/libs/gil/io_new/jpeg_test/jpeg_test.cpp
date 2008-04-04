// jpeg_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/io_new/jpeg_all.hpp>

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_CASE( read_image_info_test )
{
    std::string filename( "..\\test_images\\jpg\\found online\\test.jpg" );

    {
        image_read_info< jpeg_tag > info = boost::gil::read_image_info( filename
                                                                      , jpeg_tag() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        image_read_info< jpeg_tag > info = boost::gil::read_image_info( in
                                                                      , jpeg_tag() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    {
        string filename( "..\\test\\jpg\\write_view_test.jpg" );

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), jpeg_tag() );
    }
}
