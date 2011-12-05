//#define BOOST_TEST_MODULE jpeg_read_test_module
#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_SUITE( jpeg_test )

BOOST_AUTO_TEST_CASE( read_header_test )
{
    {
        image_read_info< tag_t > info = read_image_info( jpeg_filename
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._width         , 136u );
        BOOST_CHECK_EQUAL( info._height        , 98u  );

        BOOST_CHECK_EQUAL( info._num_components, 3         );
        BOOST_CHECK_EQUAL( info._color_space   , JCS_YCbCr );

        BOOST_CHECK_EQUAL( info._data_precision, 8         );
    }
}

BOOST_AUTO_TEST_CASE( read_pixel_density_test )
{
    image_read_info< tag_t > info = read_image_info( jpeg_in + "EddDawson/36dpi.jpg"
                                                   , tag_t()
                                                   );

    rgb8_image_t img;
    read_image( jpeg_in + "EddDawson/36dpi.jpg", img, jpeg_tag() );

    image_write_info< jpeg_tag > write_settings;
    write_settings.set_pixel_dimensions( info._width, info._height, info._pixel_width_mm, info._pixel_height_mm );

    stringstream in_memory( ios_base::in | ios_base::out | ios_base::binary );
    write_view( in_memory, view( img ), write_settings );

    image_read_info< tag_t > info2 = read_image_info( in_memory
                                                    , tag_t()
                                                    );

    // Because of rounding the two results differ slightly.
    if(  std::abs( info._pixel_width_mm  - info2._pixel_width_mm  ) > 10.0
      || std::abs( info._pixel_height_mm - info2._pixel_height_mm ) > 10.0
      )
    {
        BOOST_CHECK_EQUAL( 0, 1 );
    }
}

BOOST_AUTO_TEST_CASE( read_reference_images_test )
{
    {
        typedef rgb8_image_t image_t;
        image_t img;

        read_image( jpeg_filename
                  , img
                  , tag_t()
                  );

        write_view( jpeg_out + "rgb8_test.jpg"
                  , view( img )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_CASE( dct_method_read_test )
{
    {
        typedef rgb8_image_t image_t;
        image_t img;

        image_read_settings< jpeg_tag > settings;
        settings._dct_method = jpeg_dct_method::fast;

        read_image( jpeg_filename
                  , img
                  , settings
                  );

        write_view( jpeg_out + "fast_dct_read_test.jpg"
                  , view( img )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
