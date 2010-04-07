#ifndef __TIFF_TILED_MACROS_HPP__
#define __TIFF_TILED_MACROS_HPP__

#include <boost/gil/extension/io_new/tiff_all.hpp>

#include "paths.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

typedef boost::gil::tiff_tag tag_t;

#define GENERATE_TILE_STRIP_COMPARISON_RGB_BIT_ALIGNED(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(read_tile_and_compare_with_rgb_strip_,n), bit_bit_aligned) )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      std::string filename_strip( tiff_in_GM + "tiger-rgb-strip-" + BOOST_PP_STRINGIZE(data) + "-" ); \
      std::string filename_tile ( tiff_in_GM + "tiger-rgb-tile-"  + BOOST_PP_STRINGIZE(data) + "-" ); \
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

#endif // __TIFF_TILED_MACROS_HPP__
