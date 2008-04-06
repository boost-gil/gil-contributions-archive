// tiff_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/gil/extension/io_new/tiff_all.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost::gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_CASE( read_image_info_test )
{
    string filename( "..\\test_images\\tiff\\test.tif" );

    {
        image_read_info< tag_t > info = read_image_info( filename
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 256 );
        BOOST_CHECK_EQUAL( info._height, 192 );
    }

    {
        // An ifstream cannot be used to construct a device_type.
        // There is no way to get a TIFF* from a ifstream.

/*
        ifstream in( filename.c_str(), ios::in | ios::binary );
        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );
*/
    }

    {

    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    string filename( "..\\test_images\\tiff\\test.tif" );

    {
        rgb8_image_t img;
        read_and_convert_image( filename, img, tag_t() );
    }

    {
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    {
        string filename( "..\\test\\tiff\\test.tif" );

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), tiff_tag() );
    }
}
