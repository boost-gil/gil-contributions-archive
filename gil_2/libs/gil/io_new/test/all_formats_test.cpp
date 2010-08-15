// Test will include all format's headers and load and write some images.
// This test is more of a compilation test.


#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;


BOOST_AUTO_TEST_SUITE( all_formats_test )

BOOST_AUTO_TEST_CASE( non_bit_aligned_image_test )
{
    {
        rgb8_image_t img;
        read_image( bmp_filename, img, bmp_tag() );
        write_view( bmp_out + "all_formats_test.bmp", view( img ), bmp_tag() );
    }

    {
        rgb8_image_t img;
        read_image( jpeg_filename, img, jpeg_tag() );
        write_view( jpeg_out + "all_formats_test.jpg", view( img ), jpeg_tag() );
    }

    {
        rgba8_image_t img;
        read_image( png_filename, img, png_tag() );
        write_view( png_out + "all_formats_test.png", view( img ), png_tag() );
    }

    {
        rgb8_image_t img;
        read_image( pnm_filename, img, pnm_tag() );
        write_view( pnm_out + "all_formats_test.pnm", view( img ), pnm_tag() );
    }

    {
        rgb8_image_t img;
        read_image( tiff_filename, img, tiff_tag() );
        write_view( tiff_out + "all_formats_test.tif", view( img ), tiff_tag() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
