/********************************************************
 *
 * This test file will test palette tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

using namespace boost;

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_AUTO_TEST_CASE( read_palette_tile_and_strip_01 )
{
    using namespace std;
    using namespace boost;
    using namespace gil;

    string filename_strip( tiff_in_GM + "tiger-palette-strip-08.tif" );
    string filename_tile ( tiff_in_GM + "tiger-palette-tile-08.tif"  );

    rgb16_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
}

BOOST_AUTO_TEST_SUITE_END()
