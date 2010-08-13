
/**************************************************
 *
 * This test file will test tiled tiff reading
 *
 **************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include "paths.hpp"

#include <boost/test/unit_test.hpp>

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

// Gray images
BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_minisblack_strip_8bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-minisblack-strip-08.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-minisblack-tile-08.tif"  );
    {
        typedef gray8_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}

BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_minisblack_strip_16bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-minisblack-strip-16.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-minisblack-tile-16.tif"  );
    {
        typedef gray16_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}

BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_minisblack_strip_32bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-minisblack-strip-32.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-minisblack-tile-32.tif"  );
    {
        typedef gray32_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}

// Palette images
/*
  // Does not work: there is currently no tiled reader for palette images
BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_palette_strip_8bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-palette-strip-08.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-palette-tile-08.tif"  );
    {
        typedef rgb16_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}
*/

    /*
// RGB contig 
BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_rgb_contig_strip_32bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-rgb-strip-contig-32.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-rgb-tile-contig-32.tif"  );
    {
        typedef rgb32_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}
*/

// RGB planar
BOOST_AUTO_TEST_CASE( read_tile_and_compare_with_rgb_planar_strip_32bit )
{
    std::string filename_strip( tiff_in_GM + "tiger-rgb-strip-planar-32.tif" );
    std::string filename_tile ( tiff_in_GM + "tiger-rgb-tile-planar-32.tif"  );
    {
        typedef rgb32_image_t image_t;
        image_t img_strip, img_tile;

        read_image( filename_strip, img_strip, tag_t() );
        read_image( filename_tile, img_tile, tag_t() );

        BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true);
    }
}

// todo Olivier: move this test
BOOST_AUTO_TEST_CASE( write_tile )
{
    std::string filename ( tiff_in_GM + "tiger-minisblack-tile-09.tif"  );
    {
        /*
        typedef gray16_image_t image_t;
        image_t img_strip;

        read_image( filename, img_strip, tag_t() );

        image_write_info<tag_t> info;
        info._is_tiled = true;
        info._tile_width = info._tile_length = 16;

        write_view( "/home/olivier/Bureau/test.tif", view(img_strip), info );
        */
        typedef bit_aligned_image1_type< 9,       gray_layout_t >::type image_t;
        image_t img_strip;

        read_image( filename, img_strip, tag_t() );

        image_write_info<tag_t> info;
        info._is_tiled = true;
        info._tile_width = info._tile_length = 16;

        write_view( "/home/olivier/Bureau/test_bitaligned.tif", view(img_strip), info );
    }
}

BOOST_AUTO_TEST_SUITE_END()
