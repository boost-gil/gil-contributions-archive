#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_SUITE( jpeg_test )

BOOST_AUTO_TEST_CASE( file_format_test )
{
    std::string filename( jpeg_in + "found online\\test.jpg" );

    {
        typedef rgb8_image_t image_t;
        image_t img;

        read_image( filename
                  , img
                  , tag_t()
                  );

        write_view( jpeg_out + "test5.jpg"
                  , view( img )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_CASE( dct_method_test )
{
    std::string filename( jpeg_in + "found online\\test.jpg" );

    {
        typedef rgb8_image_t image_t;
        image_t img;

        image_read_settings< jpeg_tag > settings;
        settings._dct_method = jpeg_dct_method::fast;

        read_image( filename
                  , img
                  , settings
                  );

        write_view( jpeg_out + "test6.jpg"
                  , view( img )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
