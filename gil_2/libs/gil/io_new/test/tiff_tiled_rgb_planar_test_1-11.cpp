/********************************************************
 *
 * This test file will test RGB planar tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(1, 11, GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED_RGB, (rgb,planar) )

BOOST_AUTO_TEST_SUITE_END()
