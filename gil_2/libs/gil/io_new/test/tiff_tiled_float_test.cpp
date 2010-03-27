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

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

namespace boost { namespace gil {
typedef float   bits32F;
GIL_DEFINE_BASE_TYPEDEFS(32F,gray)
GIL_DEFINE_ALL_TYPEDEFS (32F,gray)
GIL_DEFINE_BASE_TYPEDEFS(32F,rgb)
GIL_DEFINE_ALL_TYPEDEFS (32F,rgb)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(32F, dev3n, devicen_t<3>, devicen_layout_t<3>)

typedef double  bits64F;
GIL_DEFINE_BASE_TYPEDEFS(64F,gray)
GIL_DEFINE_ALL_TYPEDEFS (64F,gray)
GIL_DEFINE_BASE_TYPEDEFS(64F,rgb)
GIL_DEFINE_ALL_TYPEDEFS (64F,rgb)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(64F, dev3n, devicen_t<3>, devicen_layout_t<3>)
} }

BOOST_AUTO_TEST_SUITE( tiff_test )

#define GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(\
                                       BOOST_PP_CAT(\
                                                    BOOST_PP_CAT(\
                                                                 BOOST_PP_CAT(read_tile_and_compare_with_,data)\
                                                                 ,_float_strip_contig_)\
                                                    ,n)\
                                       ,bit)\
                          )\
    { \
      std::string filename_strip; \
      std::string filename_tile ; \
      if(BOOST_PP_STRINGIZE(data)=="gray") \
      { \
        filename_strip = tiff_in_GM + "tiger-minisblack-float-strip-"; \
        filename_tile  = tiff_in_GM + "tiger-minisblack-float-tile-" ; \
      } \
      else \
      { \
        filename_strip = tiff_in_GM + "tiger-rgb-float-strip-contig-"; \
        filename_tile  = tiff_in_GM + "tiger-rgb-float-tile-contig-" ; \
      } \
      filename_strip = filename_strip + BOOST_PP_STRINGIZE(n) + ".tif"; \
      filename_tile = filename_tile   + BOOST_PP_STRINGIZE(n) + ".tif"; \
      typedef data####n##F_image_t image_t; \
      image_t img_strip, img_tile; \
      read_image( filename_strip, img_strip, tag_t() ); \
      read_image( filename_tile,  img_tile,  tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
    } \

//GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 24, rgb);
//GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 16, rgb);
GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 32, rgb);
GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 64, rgb);
GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 32, gray);
GENERATE_TILE_STRIP_COMPARISON_FLOAT_CONTIG(dummy, 64, gray);


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
