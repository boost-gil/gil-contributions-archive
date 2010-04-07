/********************************************************
 *
 * This test file will test RGB planar tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(1, 33, GENERATE_TILE_STRIP_COMPARISON_RGB_BIT_ALIGNED, planar)
GENERATE_TILE_STRIP_COMPARISON_RGB_BIT_ALIGNED(dummy, 64, planar);

BOOST_AUTO_TEST_SUITE_END()
