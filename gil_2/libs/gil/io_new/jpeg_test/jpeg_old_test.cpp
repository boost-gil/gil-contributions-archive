// jpeg_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_io_old.hpp>

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_CASE( read_image_info_test )
{
    std::string filename( "..\\test_images\\jpg\\found online\\test.jpg" );

    {
        jpeg_read_dimensions( filename.c_str() );
    }
}
