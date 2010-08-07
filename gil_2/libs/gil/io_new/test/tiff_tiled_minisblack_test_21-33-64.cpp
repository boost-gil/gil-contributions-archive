/********************************************************
 *
 * This test file will test gray tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(21, 32, GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED_MINISBLACK, minisblack )
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED_MINISBLACK( , 64, minisblack );

BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_minisblack_strip_32 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-minisblack-strip-32.tif" );
    string filename_tile ( tiff_in_GM + "tiger-minisblack-tile-32.tif"  );

    typedef pixel< unsigned int, gray_layout_t > gray32_pixel_t;
    image< gray32_pixel_t > img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
}

BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_minisblack_strip_64 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-minisblack-strip-64.tif" );
    string filename_tile ( tiff_in_GM + "tiger-minisblack-tile-64.tif"  );

    typedef pixel< unsigned __int64, gray_layout_t > gray64_pixel_t;
    image< gray64_pixel_t > img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
}

BOOST_AUTO_TEST_SUITE_END()
