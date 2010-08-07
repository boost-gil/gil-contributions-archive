#ifndef __TIFF_TILED_MACROS_HPP__
#define __TIFF_TILED_MACROS_HPP__

#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include "paths.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

typedef boost::gil::tiff_tag tag_t;

#define GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT( \
                                BOOST_PP_CAT( \
                                    BOOST_PP_CAT( \
                                        BOOST_PP_CAT( \
                                            BOOST_PP_CAT( read_tile_and_compare_with_ \
                                                        , BOOST_PP_TUPLE_ELEM(2,0,data) \
                                                        ) \
                                                , BOOST_PP_TUPLE_ELEM(2,1,data) \
                                                ) \
                                            ,_strip_ \
                                            ) \
                                        ,n \
                                        ) \
                                      , bit_bit_aligned \
                                      ) \
                        )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      string filename_strip( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2,0,data)) + "-strip-" + BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2,1,data)) + "-" ); \
      string filename_tile ( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2,0,data)) + "-tile-"  + BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2,1,data)) + "-" ); \
      string padding(""); \
      if(BOOST_PP_LESS(n, 10)==1) \
        padding = "0"; \
      filename_strip = filename_strip + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      filename_tile  = filename_tile  + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      typedef mpl::vector< bit_aligned_image3_type< n, n, n, rgb_layout_t  >::type \
                         , bit_aligned_image1_type< n,       gray_layout_t >::type \
                         , gray32f_image_t \
                         , gil::detail::gray64f_image_t \
                         , rgb32f_image_t  \
                         , gil::detail::rgb64f_image_t > image_types; \
      any_image< image_types > img_strip, img_tile; \
      read_image( filename_strip, img_strip, tag_t() ); \
      read_image( filename_tile,  img_tile,  tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
      rgb8_image_t dst( view( img_strip ).dimensions() ); \
      copy_and_convert_pixels( view( img_strip ), view( dst )); \
    } \

// Special case for minisblack images
#define GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED_MINISBLACK(z, n, data)\
    BOOST_AUTO_TEST_CASE( BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(read_tile_and_compare_with_,data),_strip_),n), bit_bit_aligned) )\
    { \
      using namespace std; \
      using namespace boost; \
      using namespace gil; \
      string filename_strip( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(data) + "-strip-" ); \
      string filename_tile ( tiff_in_GM + "tiger-" + BOOST_PP_STRINGIZE(data) + "-tile-"  ); \
      string padding(""); \
      if(BOOST_PP_LESS(n, 10)==1) \
        padding = "0"; \
      filename_strip = filename_strip + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      filename_tile  = filename_tile  + padding + BOOST_PP_STRINGIZE(n) + ".tif"; \
      typedef mpl::vector< bit_aligned_image3_type< n, n, n, rgb_layout_t  >::type \
                         , bit_aligned_image1_type< n,       gray_layout_t >::type \
                         , gray32f_image_t \
                         , gil::detail::gray64f_image_t \
                         , rgb32f_image_t  \
                         , gil::detail::rgb64f_image_t > image_types; \
      any_image< image_types > img_strip, img_tile; \
      read_image( filename_strip, img_strip, tag_t() ); \
      read_image( filename_tile,  img_tile,  tag_t() ); \
      BOOST_CHECK_EQUAL( equal_pixels( const_view(img_strip), const_view(img_tile) ), true); \
    } \



#endif // __TIFF_TILED_MACROS_HPP__
