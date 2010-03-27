/********************************************************
 *
 * This test file will test RGB contig tiled tiff reading
 *
 *******************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include "paths.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

#define GENERATE_TILE_STRIP_COMPARISON_RGB_CONTIG_BIT_ALIGNED(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(read_tile_and_compare_with_rgb_strip_contig_,n), bit_bit_aligned) )\
    { \
      std::string filename_strip( tiff_in_GM + "tiger-rgb-strip-contig-" ); \
      std::string filename_tile ( tiff_in_GM + "tiger-rgb-tile-contig-"  ); \
      typedef bit_aligned_image3_type< n, n, n, rgb_layout_t >::type image_t; \
      if(BOOST_PP_GREATER_EQUAL(n, 10)==1) \
      { \
        filename_strip = filename_strip + BOOST_PP_STRINGIZE(n) + ".tif"; \
        filename_tile = filename_tile   + BOOST_PP_STRINGIZE(n) + ".tif"; \
      } \
      else \
      { \
        filename_strip = filename_strip + BOOST_PP_STRINGIZE(BOOST_PP_CAT( 0 , n)) + ".tif"; \
        filename_tile = filename_tile   + BOOST_PP_STRINGIZE(BOOST_PP_CAT( 0 , n)) + ".tif"; \
      } \
      image_t img_strip, img_tile; \
      read_image( filename_strip, img_strip, tag_t() ); \
      read_image( filename_tile,  img_tile,  tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
    } \

BOOST_PP_REPEAT_FROM_TO(1, 33, GENERATE_TILE_STRIP_COMPARISON_RGB_CONTIG_BIT_ALIGNED, ~)
GENERATE_TILE_STRIP_COMPARISON_RGB_CONTIG_BIT_ALIGNED(dummy, 64, dummy);

BOOST_AUTO_TEST_SUITE_END()
