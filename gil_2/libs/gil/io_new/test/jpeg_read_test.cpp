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

        BOOST_CHECK_EQUAL( info._width         , 136 );
        BOOST_CHECK_EQUAL( info._height        , 98  );

        BOOST_CHECK_EQUAL( info._num_components, 3         );
        BOOST_CHECK_EQUAL( info._color_space   , JCS_YCbCr );

        BOOST_CHECK_EQUAL( info._data_precision, 8         );
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
