#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_CASE( pnm_old_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = pnm_read_dimensions( pnm_filename );

        BOOST_CHECK_EQUAL( dim.x, 200 );
        BOOST_CHECK_EQUAL( dim.y, 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_old_read_image_test )
{
    {
        rgb8_image_t img;
        pnm_read_image( pnm_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_old_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        pnm_read_and_convert_image( pnm_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_old_read_view_test )
{
    {
        gray8_image_t img( 200, 200 );
        pnm_read_view( pnm_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( pnm_old_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 200, 200 );
        pnm_read_and_convert_view( pnm_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( pnm_old_write_view_test )
{
    {
        string filename( pnm_out + "test5.pnm" );

        gray8_image_t img( 200, 200);
        pnm_write_view( filename, view( img ) );
    }
}
