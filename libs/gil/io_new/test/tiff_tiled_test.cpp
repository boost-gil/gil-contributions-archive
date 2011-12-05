
/**************************************************
 *
 * This test file will test tiled tiff reading
 *
 **************************************************/
//#define BOOST_TEST_MODULE tiff_tiled_test_module
#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/io_new/tiff_read.hpp>
#include "paths.hpp"

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_AUTO_TEST_CASE( read_tile_infos_test )
{
    {
        image_read_info< tag_t > info = read_image_info( tiff_in_GM + "tiger-minisblack-float-tile-16.tif"
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._tile_width , 16 );
        BOOST_CHECK_EQUAL( info._tile_length, 16 );
    }

    {
        image_read_info< tag_t > info = read_image_info( tiff_in_GM + "tiger-minisblack-tile-08.tif"
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._tile_width , 16 );
        BOOST_CHECK_EQUAL( info._tile_length, 16 );
    }

    {
        image_read_info< tag_t > info = read_image_info( tiff_in_GM + "tiger-palette-tile-08.tif"
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._tile_width , 16 );
        BOOST_CHECK_EQUAL( info._tile_length, 16 );
    }

    {
        image_read_info< tag_t > info = read_image_info( tiff_in_GM + "tiger-rgb-tile-contig-08.tif"
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._tile_width , 16 );
        BOOST_CHECK_EQUAL( info._tile_length, 16 );
    }

    {
        image_read_info< tag_t > info = read_image_info( tiff_in_GM + "tiger-rgb-tile-planar-08.tif"
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._tile_width , 16 );
        BOOST_CHECK_EQUAL( info._tile_length, 16 );
    }
}

BOOST_AUTO_TEST_SUITE_END()
