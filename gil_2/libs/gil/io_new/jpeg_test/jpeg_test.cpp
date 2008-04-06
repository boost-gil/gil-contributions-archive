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
        image_read_info< jpeg_tag > info = read_image_info( filename
                                                          , jpeg_tag() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        image_read_info< jpeg_tag > info = read_image_info( in
                                                          , jpeg_tag() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        FILE* file = fopen( filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = boost::gil::read_image_info( file
                                                                   , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    std::string filename( "..\\test_images\\jpg\\found online\\test.jpg" );

    {
        rgb8_image_t img;
        read_and_convert_image( filename, img, jpeg_tag() );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, jpeg_tag() );
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
