
#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/sdl2/service.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

using namespace boost;
using namespace gil;
using namespace sdl;

BOOST_AUTO_TEST_CASE( test_sdl_basic )
{
    service sdl_service;

    sdl_service.run();
}
