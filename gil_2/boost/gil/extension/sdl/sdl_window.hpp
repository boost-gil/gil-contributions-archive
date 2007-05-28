// Copyright 2007 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <boost/scoped_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>

#include "sdl_events.hpp"
#include "sdl_timer.hpp"
#include "user_events.hpp"
#include "message_queue.h"

namespace boost { namespace gil { namespace sdl { 

typedef detail::sdl_event_base sdl_event_t;
typedef ogx::message_queue< sdl_event_t > queue_t;

class sdl_window_base
{
protected:

   sdl_window_base( int width
                  , int height )
   : _queue( NULL )
   , _width ( width  )
   , _height( height )
   {
      _screen = SDL_SetVideoMode( _width
                                , _height
                                , 32
                                , SDL_SWSURFACE );

      if( _screen == NULL )
      {
         throw std::runtime_error( "Couldn't create SDL window" );
      }
   }

   void lock()
   {
      if( _screen && SDL_MUSTLOCK( _screen ))
      {
         if( SDL_LockSurface( _screen ) < 0 ) 
         {
            return;
         }
      }
   }

   void unlock()
   {
      if( _screen && SDL_MUSTLOCK( _screen )) 
      {
         SDL_UnlockSurface( _screen );
      }

      // Tell SDL to update the whole screen
      SDL_UpdateRect( _screen
                    , 0
                    , 0
                    , _width
                    , _height );
   }

   queue_t* get_queue() { return _queue; }

private:

   void set_queue( queue_t& queue )
   {
      _queue = &queue;
   }

protected:

   int _width;
   int _height;

   SDL_Surface* _screen;

private:

   queue_t* _queue;

   friend class sdl_service;
};

// active object for displaying images

template < typename KEYBOARD_EVENTS = detail::default_keyboard_events
         , typename REDRAW_EVENT    = detail::default_redraw_event
         , typename TIMER_EVENT     = detail::default_timer_event
         , typename QUIT_EVENT      = detail::default_quit_event
         >
class sdl_window : public sdl_window_base
                 , public detail::sdl_timer_base< TIMER_EVENT >
                 , public KEYBOARD_EVENTS
                 , public REDRAW_EVENT
                 , public QUIT_EVENT
{
public:

   sdl_window( int width
             , int height )
   : sdl_window_base( width
                    , height )
   , _cancel( false )
   {
      _thread.reset( new boost::thread( boost::bind( _run
                                                   , this )));
   }

   ~sdl_window()
   {
      cancel();
      _thread->join();
   }

   void cancel()
   {
      boost::mutex::scoped_lock oLock( _sentinel );

      _cancel = true;
   }

private:

   void _run()
   {
      boost::shared_ptr< sdl_event_t > e;

      while( _cancel == false )
      {
         if( get_queue() )
         {
            get_queue()->dequeue( e );

            switch( e->type() )
            {
               case detail::redraw_event::type:
               {
                  lock();

                  redraw( wrap_sdl_image( _screen ));

                  unlock();

                  break;
               }

               case detail::key_up_event::type:
               {
                  lock();

                  if( key_up() == true )
                  {
                     redraw( wrap_sdl_image( _screen ));
                  }

                  unlock();

                  break;
               }

               case detail::quit_event::type:
               {
                  quit();

                  return;
               }
            }
         }
      }
   }

private:

   typedef boost::scoped_ptr<boost::thread> thread_t;
   thread_t _thread;

   mutable boost::mutex _sentinel;

   bool _cancel;
};


} } } // namespace boost::gil::sdl

#endif // SDL_WINDOW_HPP
