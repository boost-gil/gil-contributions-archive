/********************************************************
 *
 * This test file will test RGB planar tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(21, 32, GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED, (rgb,planar) )

// TODO: move in a rgb_float test file
BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_rgb_float_planar_strip_32 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-rgb-float-tile-planar-32.tif" );
    string filename_tile ( tiff_in_GM + "tiger-rgb-float-strip-planar-32.tif" );

    rgb32f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
}

BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_rgb_float_planar_strip_64 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-rgb-float-tile-planar-64.tif" );
    string filename_tile ( tiff_in_GM + "tiger-rgb-float-strip-planar-64.tif" );

    gil::detail::rgb64f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
}

BOOST_AUTO_TEST_SUITE_END()
