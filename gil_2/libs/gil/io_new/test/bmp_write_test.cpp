#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/detail/typedefs.hpp>
#include <boost/gil/extension/io_new/bmp_write.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

namespace bmp_test {

BOOST_AUTO_TEST_CASE( write_test )
{
    // test writing all supported image types
    {
        rgb8_image_t img( 10, 10 );
        write_view( bmp_out + "write_test_rgb.bmp", view( img ), bmp_tag() );
    }

    {
        rgba8_image_t img( 10, 10 );
        write_view( bmp_out + "write_test_rgba.bmp", view( img ), bmp_tag() );
    }
}

} // namespace bmp_test
