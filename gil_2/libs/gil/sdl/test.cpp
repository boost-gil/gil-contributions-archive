// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "boost/gil/gil_all.hpp"

#include "boost/gil/extension/io/bmp_io.hpp"
#include "boost/gil/extension/sdl/sdl_wrapper.hpp"

using namespace std;
using namespace boost;
using namespace gil;
using namespace sdl;

class my_timer : public timer_event_base
{
public:

   virtual void time_elapsed()
   {
      std::cout << "time elapsed" << std::endl;
   }
};

// called from window's thread
class draw_something : public keyboard_event_base
{
public:

   draw_something( const rgb8_view_t& view ) : _view( view ) {}

   virtual void key_up( const bgra8_view_t& sdl_view )
   {
      copy_pixels( color_converted_view<bgra8_pixel_t>( _view )
                 , sdl_view );
   }

private:

   const rgb8_view_t& _view;
};

int main( int argc, char* argv[] )
{
   sdl_service ss;

   rgb8_image_t img;
   bmp_read_image( "flower.bmp", img );

   draw_something painter( view( img ));
   my_timer mytimer;
   sdl_window win( view( img ).width()
                 , view( img ).height()
                 , NULL
                 , &painter
                 , &mytimer );

   win.set_timer( 1000 );

   ss.add_window( win );
   ss.run();

	return 0;
}
