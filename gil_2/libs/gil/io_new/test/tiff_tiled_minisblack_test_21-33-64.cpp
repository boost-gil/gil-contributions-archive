/********************************************************
 *
 * This test file will test gray tiled tiff reading
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>

#include "tiff_tiled_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(21, 33, GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED, (minisblack,) )
GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED( , 64, (minisblack,) );

BOOST_AUTO_TEST_SUITE_END()
