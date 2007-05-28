// Copyright 2007 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef SDL_TIMER_HPP
#define SDL_TIMER_HPP

#include <SDL.h>

#include "user_events.hpp"

namespace boost { namespace gil { namespace sdl { namespace detail {

template< typename TIMER_EVENT = detail::default_timer_event
        >
class sdl_timer_base : public TIMER_EVENT
{
public:

   sdl_timer_base()
   : _timer_id( 0 )
   , _timer_init( false )
   {}

   ~sdl_timer_base()
   {
      if( _timer_init )
      {
         SDL_RemoveTimer( _timer_id );
      }
   }

   // interval in ms
   void set_timer( unsigned int interval )
   {
      if( _timer_init == false )
      {
         _timer_id = SDL_AddTimer( interval
                                 , &sdl_timer_base::timer_callback
                                 , this                              );

         _timer_init = true;
      }
      else
      {
         throw std::runtime_error( "Timer is already running." );
      }   
   }

private:

   static Uint32 timer_callback( Uint32 interval, void *param )
   {
      sdl_timer_base* p = (sdl_timer_base*) param;

      if( p && p->time_elapsed() )
      {
         //redraw( wrap_sdl_image( _screen ));
      }

      // The return value needs to be different from the interval parameter.
      // Otherwise the timer will stop.
      return ++interval;
   }

private:

   bool _timer_init;

   SDL_TimerID _timer_id;
};

} } } } // namespace boost::gil::sdl::detail

#endif //SDL_TIMER_HPP