#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/detail/typedefs.hpp>
#include <boost/gil/extension/io_new/bmp_write.hpp>

#include <boost/test/unit_test.hpp>

#include "mandel_view.hpp"
#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef bmp_tag tag_t;

BOOST_AUTO_TEST_SUITE( bmp_test )

BOOST_AUTO_TEST_CASE( write_test )
{
    // test writing all supported image types
    {
        write_view( bmp_out + "rgb8_test.bmp"
                  , create_mandel_view( 200, 200
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        write_view( bmp_out + "rgba8_test.bmp"
                  , create_mandel_view( 200, 200
                                      , rgba8_pixel_t( 0,   0, 255, 0 )
                                      , rgba8_pixel_t( 0, 255,   0, 0 )
                                      )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
