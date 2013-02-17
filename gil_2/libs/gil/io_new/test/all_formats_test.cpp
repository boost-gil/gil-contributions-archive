// Test will include all format's headers and load and write some images.
// This test is more of a compilation test.


#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>
#include <boost/gil/extension/io_new/targa_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"
#include "write_test_image.hpp"

using namespace std;
using namespace boost::gil;


BOOST_AUTO_TEST_SUITE( all_formats_test )

BOOST_AUTO_TEST_CASE( non_bit_aligned_image_test )
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_READING_IMAGES
    {
        rgb8_image_t img;
        read_test_image( bmp_filename, img, bmp_tag() );
        write_test_view( bmp_out + "all_formats_test.bmp", view( img ), bmp_tag() );
    }

    {
        rgb8_image_t img;
        read_test_image( jpeg_filename, img, jpeg_tag() );
        write_test_view( jpeg_out + "all_formats_test.jpg", view( img ), jpeg_tag() );
    }

    {
        rgba8_image_t img;
        read_test_image( png_filename, img, png_tag() );
        write_test_view( png_out + "all_formats_test.png", view( img ), png_tag() );
    }

    {
        rgb8_image_t img;
        read_test_image( pnm_filename, img, pnm_tag() );
        write_test_view( pnm_out + "all_formats_test.pnm", view( img ), pnm_tag() );
    }

    {
        rgb8_image_t img;
        read_test_image( targa_filename, img, targa_tag() );
        write_test_view( targa_out + "all_formats_test.tga", view( img ), targa_tag() );
    }

    {
        rgba8_image_t img;
        read_test_image( tiff_filename, img, tiff_tag() );
        write_test_view( tiff_out + "all_formats_test.tif", view( img ), tiff_tag() );
    }
#endif // BOOST_GIL_IO_TEST_ALLOW_READING_IMAGES
}

BOOST_AUTO_TEST_SUITE_END()
