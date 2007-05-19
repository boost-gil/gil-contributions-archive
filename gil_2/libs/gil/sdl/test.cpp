// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "boost/any.hpp"

#include "boost/thread/xtime.hpp"

#include "boost/gil/gil_all.hpp"

#include "boost/gil/extension/io/bmp_io.hpp"
#include "boost/gil/extension/sdl/sdl_wrapper.hpp"

using namespace std;
using namespace boost;
using namespace gil;
using namespace sdl;

// called from window's thread
class draw_something : public keyboard_event_base
{
public:

   draw_something( const rgb8_view_t& view ) : _view( view ) {}

   virtual void key_up( SDL_Surface* screen )
   {
      for( int y = 0; y < screen->h; ++y )
      {
         rgb8_view_t::x_iterator x_it = _view.row_begin( y );

         for( int x = 0; x < screen->w; ++x )
         {
            const rgb8_pixel_t& p = x_it[x];

            set_pixel( p, screen, x, y );
         }
      }
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

   sdl_window win( view( img ).width()
                 , view( img ).height()
                 , NULL
                 , &painter               );

   ss.add_window( win );
   ss.run();

	return 0;
}
