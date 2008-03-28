// read_info_test_vc8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( read_image_info_using_string )
{
    {
        std::string filename( "..\\test_images\\jpg\\found online\\test.jpg" );

        using namespace boost::gil;

        image_read_info< jpeg_tag > info = boost::gil::read_image_info( filename
                                                                      , jpeg_tag() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        std::string filename( "..\\test_images\\png\\wikipedia\\test.png" );

        using namespace boost::gil;

        image_read_info< png_tag > info = boost::gil::read_image_info( filename
                                                                     , png_tag() );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }

    {
        std::string filename( "..\\test_images\\png\\wikipedia\\test.png" );

        using namespace boost::gil;

        image_read_info< png_tag > info = boost::gil::read_image_info( filename
                                                                     , png_tag() );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }

    {
        std::string filename( "..\\test_images\\tiff\\test.tif" );

        using namespace boost::gil;

        image_read_info< tiff_tag > info = boost::gil::read_image_info( filename
                                                                      , tiff_tag() );

        BOOST_CHECK_EQUAL( info._width , 256 );
        BOOST_CHECK_EQUAL( info._height, 192 );
    }
}
