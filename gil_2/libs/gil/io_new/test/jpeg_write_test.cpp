#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "mandel_view.hpp"
#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_SUITE( jpeg_test )

BOOST_AUTO_TEST_CASE( write_test )
{
    // test writing all supported image types

    {
        write_view( jpeg_out + "gray8_test.jpg"
                  , create_mandel_view( 200, 200
                                      , gray8_pixel_t( 0   )
                                      , gray8_pixel_t( 255 )
                                      )
                  , tag_t()
                  );
    }

    {
        write_view( jpeg_out + "rgb8_test.jpg"
                  , create_mandel_view( 200, 200
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        write_view( jpeg_out + "cmyk8_test.jpg"
                  , create_mandel_view( 200, 200
                                      , cmyk8_pixel_t( 0,   0, 255, 127 )
                                      , cmyk8_pixel_t( 0, 255,   0, 127 )
                                      )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
