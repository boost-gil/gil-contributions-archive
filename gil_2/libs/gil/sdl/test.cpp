// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "boost/gil/extension/sdl/sdl_wrapper.hpp"

using namespace std;
using namespace boost;
using namespace gil;

int main( int argc, char* argv[] )
{
   sdl_wrapper::get().init_sdl();

   rgb8_image_t img;
   bmp_read_image( "flower.bmp", img );

   sdl_window win( view( img ).width()
                 , view( img ).height() );

   boost::xtime xt;
   xtime_get(&xt, boost::TIME_UTC);
   xt.sec += 10;    // change xt to next second
   thread::sleep( xt );

	return 0;
}

