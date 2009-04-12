
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/io_new/png_write.hpp>
#include <boost/gil/extension/io_new/tiff_write.hpp>

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_CASE( simple_test )
{
    typedef stringstream device_t;

    device_t ss( ios_base::in | ios_base::out | ios_base::binary );

    rgb8_image_t img( 100, 100 );
    write_view( ss, view( img ), tiff_tag() );
}