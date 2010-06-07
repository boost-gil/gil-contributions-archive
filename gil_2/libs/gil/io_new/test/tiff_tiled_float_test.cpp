/********************************************************
 *
 * This test file will test float tiled tiff reading
 *
 *******************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include "paths.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include "tiff_tiled_macros.hpp"

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 32, (rgb-loat,contig-) );
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 64, (rgb-loat,contig-) );
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 32, (BOOST_PP_CAT(rgb,BOOST_PP_CAT(-,float)),contig- ) );
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 64, (BOOST_PP_CAT(rgb,BOOST_PP_CAT(-,float)),contig- ) );
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 32, (BOOST_PP_CAT(minisblack,BOOST_PP_CAT(-,flot)), ) );
//GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 64, (BOOST_PP_CAT(minisblack,BOOST_PP_CAT(-,flot)), ) );


#define GENERATE_TILE_STRIP_COMPARISON_FLOAT_PLANAR(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(read_tile_and_compare_with_rgb_strip_planar_,n), bit) )\
    { \
      std::string filename_strip( tiff_in_GM + "tiger-rgb-float-strip-planar-" ); \
      std::string filename_tile ( tiff_in_GM + "tiger-rgb-float-tile-planar-"  ); \
      filename_strip = filename_strip + BOOST_PP_STRINGIZE(n) + ".tif"; \
      filename_tile = filename_tile   + BOOST_PP_STRINGIZE(n) + ".tif"; \
      typedef rgb##n##F_image_t image_t; \
      image_t img_strip, img_tile; \
      read_image( filename_strip, img_strip, tag_t() ); \
      read_image( filename_tile,  img_tile,  tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
    } \

GENERATE_TILE_STRIP_COMPARISON_FLOAT_PLANAR(dummy, 32, dummy);
GENERATE_TILE_STRIP_COMPARISON_FLOAT_PLANAR(dummy, 64, dummy);

BOOST_AUTO_TEST_SUITE_END()
