#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/png_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

namespace png_test {

std::string filename( png_in + "wikipedia\\test.png" );

BOOST_AUTO_TEST_CASE( png_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = png_read_dimensions( filename );

        BOOST_CHECK_EQUAL( dim.x, 320 );
        BOOST_CHECK_EQUAL( dim.y, 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_read_image_test )
{
    {
        rgba8_image_t img;
        png_read_image( filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        png_read_and_convert_image( filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_read_view_test )
{
    {
        rgba8_image_t img( 320, 240 );
        png_read_view( filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( png_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 320, 240 );
        png_read_and_convert_view( filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( png_write_view_test )
{
    {
        string filename( png_out + "test5.png" );

        gray8_image_t img( 320, 240 );
        png_write_view( filename, view( img ) );
    }
}

} // namespace png_test
