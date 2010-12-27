//#define BOOST_TEST_MODULE targa_write_test_module
#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/detail/typedefs.hpp>
#include <boost/gil/extension/io_new/targa_write.hpp>

#include "mandel_view.hpp"
#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef targa_tag tag_t;

BOOST_AUTO_TEST_SUITE( targa_test )

BOOST_AUTO_TEST_CASE( write_test )
{
    // test writing all supported image types
    {
        write_view( targa_out + "rgb8_test.tga"
                  , create_mandel_view( 200, 200
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        write_view( targa_out + "rgba8_test.tga"
                  , create_mandel_view( 200, 200
                                      , rgba8_pixel_t( 0,   0, 255, 0 )
                                      , rgba8_pixel_t( 0, 255,   0, 0 )
                                      )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
