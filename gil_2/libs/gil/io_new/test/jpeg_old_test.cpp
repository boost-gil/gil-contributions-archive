#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_CASE( jpeg_old_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = jpeg_read_dimensions( jpeg_filename );

        BOOST_CHECK_EQUAL( dim.x, 136 );
        BOOST_CHECK_EQUAL( dim.y, 98  );
    }
}

BOOST_AUTO_TEST_CASE( jpeg_old_read_image_test )
{
    {
        rgb8_image_t img;
        jpeg_read_image( jpeg_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }
}

BOOST_AUTO_TEST_CASE( jpeg_old_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        jpeg_read_and_convert_image( jpeg_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }
}

BOOST_AUTO_TEST_CASE( jpeg_old_read_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        jpeg_read_view( jpeg_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( jpeg_old_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        jpeg_read_and_convert_view( jpeg_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( jpeg_old_write_view_test )
{
    {
        string filename( jpeg_out + "test1.jpg" );

        gray8_image_t img( 136, 98 );
        jpeg_write_view( filename, view( img ) );
    }
}
