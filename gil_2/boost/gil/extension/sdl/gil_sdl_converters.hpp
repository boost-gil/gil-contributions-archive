// Copyright 2007 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef GIL_SDL_CONVERTERS_HPP
#define GIL_SDL_CONVERTERS_HPP

#include <SDL.h>

namespace boost { namespace gil { namespace sdl { 

inline
void set_pixel( const boost::gil::rgb8_pixel_t& pixel
              , SDL_Surface*                    screen
              , int                             x
              , int                             y       )
{
   Uint32 col = SDL_MapRGB( screen->format
                          , get_color( pixel, boost::gil::red_t()   )
                          , get_color( pixel, boost::gil::green_t() )
                          , get_color( pixel, boost::gil::blue_t() ));

   char* pos = static_cast<char*>( screen->pixels );

   pos += screen->pitch * y;
   pos += screen->format->BytesPerPixel * x;
   
   memcpy( pos, &col, screen->format->BytesPerPixel );
}


} } } // namespace boost::gil::sdl

#endif // GIL_SDL_CONVERTERS_HPP
