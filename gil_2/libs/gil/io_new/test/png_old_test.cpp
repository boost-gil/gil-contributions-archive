#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/png_io_old.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( png_test )

BOOST_AUTO_TEST_CASE( old_read_dimensions_test )
{
    {
        point2< ptrdiff_t > dim = png_read_dimensions( png_filename );

        BOOST_CHECK_EQUAL( dim.x, 320 );
        BOOST_CHECK_EQUAL( dim.y, 240 );
    }
}

BOOST_AUTO_TEST_CASE( old_read_image_test )
{
    {
        rgba8_image_t img;
        png_read_image( png_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( old_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        png_read_and_convert_image( png_filename, img );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( old_read_view_test )
{
    {
        rgba8_image_t img( 320, 240 );
        png_read_view( png_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( old_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 320, 240 );
        png_read_and_convert_view( png_filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( old_write_view_test )
{
    {
        string filename( png_out + "test5.png" );

        gray8_image_t img( 320, 240 );
        png_write_view( filename, view( img ) );
    }
}

BOOST_AUTO_TEST_CASE( old_dynamic_image_test )
{
    typedef mpl::vector< gray8_image_t
                       , gray16_image_t
                       , rgb8_image_t
                       , rgba8_image_t
                       > my_img_types;


    any_image< my_img_types > runtime_image;

    png_read_image( png_filename.c_str()
                  , runtime_image
                  );

    png_write_view( png_out + "old_dynamic_image_test.png"
                  , view( runtime_image )
                  );
}

BOOST_AUTO_TEST_SUITE_END()
