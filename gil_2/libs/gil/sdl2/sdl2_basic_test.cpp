
#include <SDL.h>

#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/sdl2/service.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

using namespace boost;
using namespace gil;
using namespace sdl;

BOOST_AUTO_TEST_CASE( test_sdl_basic )
{
    rgba8_image_t img( 640, 480 );

    service sdl_service;

    window w( view( img ));
    sdl_service.add_window( w );

    sdl_service.run();
}
