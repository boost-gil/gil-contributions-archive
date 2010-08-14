/********************************************************
 *
 * This test file will test gray tiled tiff writing
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>
#include "tiff_tiled_write_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(21, 32, GENERATE_WRITE_TILE_BIT_ALIGNED_MINISBLACK, minisblack )

BOOST_AUTO_TEST_CASE( write_tile_and_compare_with_minisblack_strip_32 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-minisblack-strip-32.tif" );

    typedef pixel< unsigned int, gray_layout_t > gray32_pixel_t;
    image< gray32_pixel_t, false > img_strip, img_saved;

    read_image( filename_strip, img_strip, tag_t() );

    image_write_info<tag_t> info;
    info._is_tiled = true;
    info._tile_width = info._tile_length = 16;

    write_view( "./temp.tif", view(img_strip), info );
    read_image( "./temp.tif", img_saved, tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_saved) ), true);
}

BOOST_AUTO_TEST_CASE( write_tile_and_compare_with_minisblack_strip_64 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-minisblack-strip-64.tif" );

    typedef pixel< uint64_t, gray_layout_t > gray64_pixel_t;
    image< gray64_pixel_t, false > img_strip, img_saved;

    read_image( filename_strip, img_strip, tag_t() );

    image_write_info<tag_t> info;
    info._is_tiled = true;
    info._tile_width = info._tile_length = 16;

    write_view( "./temp.tif", view(img_strip), info );
    read_image( "./temp.tif", img_saved, tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_saved) ), true); \
}

BOOST_AUTO_TEST_SUITE_END()
