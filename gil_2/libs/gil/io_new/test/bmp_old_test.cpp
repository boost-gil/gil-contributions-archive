#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_CASE( bmp_old_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = bmp_read_dimensions( bmp_filename );

        BOOST_CHECK_EQUAL( dim.x, 127 );
        BOOST_CHECK_EQUAL( dim.y, 64 );
    }
}

BOOST_AUTO_TEST_CASE( bmp_old_read_image_test )
{
    {
        rgba8_image_t img;
        bmp_read_image( bmp_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }
}

BOOST_AUTO_TEST_CASE( bmp_old_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        bmp_read_and_convert_image( bmp_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 127 );
        BOOST_CHECK_EQUAL( img.height(), 64 );
    }
}

BOOST_AUTO_TEST_CASE( bmp_old_read_view_test )
{
    {
        rgba8_image_t img( 127, 64 );
        bmp_read_view( bmp_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( bmp_old_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 127, 64 );
        bmp_read_and_convert_view( bmp_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( bmp_old_write_view_test )
{
    {
        string filename( bmp_out + "test4.bmp" );

        rgb8_image_t img( 127, 64 );
        bmp_write_view( filename, view( img ) );
    }
}
