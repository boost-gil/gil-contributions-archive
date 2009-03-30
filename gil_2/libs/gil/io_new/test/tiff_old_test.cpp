/**************************************************
 *
 * This test file will only test the old library's
 * interface. It's more of a compile time test
 * than a runtime test.
 *
 **************************************************/

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

namespace tiff_test {

string filename( tiff_in + "test.tif" );

BOOST_AUTO_TEST_CASE( tiff_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = tiff_read_dimensions( filename );

        BOOST_CHECK_EQUAL( dim.x, 200 );
        BOOST_CHECK_EQUAL( dim.y, 133 );
    }
}

BOOST_AUTO_TEST_CASE( tiff_read_image_test )
{
    {
        rgb8_image_t img;
        tiff_read_image( filename, img );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }
}

BOOST_AUTO_TEST_CASE( tiff_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        tiff_read_and_convert_image( filename, img );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }
}

BOOST_AUTO_TEST_CASE( tiff_read_view_test )
{
    {
        rgb8_image_t img( 200, 133 );
        tiff_read_view( filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( tiff_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 200, 133 );
        tiff_read_and_convert_view( filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( tiff_write_view_test )
{
    {
        string filename( tiff_out + "test3.tif" );

        gray8_image_t img( 320, 240 );
        tiff_write_view( filename, view( img ) );
    }
}

} // namespace tiff_test
