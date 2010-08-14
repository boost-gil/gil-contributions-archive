/********************************************************
 *
 * This test file will test gray tiled tiff writing
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include "tiff_tiled_read_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(11, 21, GENERATE_TILE_STRIP_COMPARISON_BIT_ALIGNED_MINISBLACK, minisblack )

BOOST_AUTO_TEST_SUITE_END()
