/********************************************************
 *
 * This test file will test float tiled tiff reading
 *
 *******************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include "paths.hpp"

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_AUTO_TEST_CASE( read_minisblack_float_tile_and_strip32 )
{
    std::string filename_strip( tiff_in_GM + "tiger-minisblack-float-strip-32.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-minisblack-float-tile-32.tif"  );

    gray32f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
}

BOOST_AUTO_TEST_CASE( read_rgb_float_tile_and_strip32 )
{
    std::string filename_strip( tiff_in_GM + "tiger-rgb-float-strip-planar-32.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-rgb-float-tile-planar-32.tif"  );

    rgb32f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
}

BOOST_AUTO_TEST_CASE( read_minisblack_float_tile_and_strip64 )
{
    std::string filename_strip( tiff_in_GM + "tiger-minisblack-float-strip-64.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-minisblack-float-tile-64.tif"  );

    gil::detail::gray64f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
}

BOOST_AUTO_TEST_CASE( read_rgb_float_tile_and_strip64 )
{
    std::string filename_strip( tiff_in_GM + "tiger-rgb-float-strip-planar-64.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-rgb-float-tile-planar-64.tif"  );

    gil::detail::rgb64f_image_t img_strip, img_tile;

    read_image( filename_strip, img_strip, tag_t() );
    read_image( filename_tile,  img_tile,  tag_t() );

    BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
}

BOOST_AUTO_TEST_SUITE_END()
