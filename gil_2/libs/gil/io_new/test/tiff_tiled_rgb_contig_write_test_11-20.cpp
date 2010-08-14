/********************************************************
 *
 * This test file will test rgb contig tiled tiff writing
 *
 *******************************************************/

#include <boost/test/unit_test.hpp>
#include "tiff_tiled_write_macros.hpp"

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_PP_REPEAT_FROM_TO(11, 21, GENERATE_WRITE_TILE_BIT_ALIGNED_RGB, rgb )

BOOST_AUTO_TEST_SUITE_END()
