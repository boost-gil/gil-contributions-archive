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

   void lock( SDL_Surface* screen )
   {
      if( screen && SDL_MUSTLOCK( screen ))
      {
         if( SDL_LockSurface( screen ) < 0 ) 
         {
            return;
         }
      }
   }

   void unlock( SDL_Surface* screen
              , int          width
              , int          height )
   {
      if( screen && SDL_MUSTLOCK( screen )) 
      {
         SDL_UnlockSurface( screen );
      }

      // Tell SDL to update the whole screen
      SDL_UpdateRect( screen
                    , 0
                    , 0
                    , width
                    , height );
   }
};

// use policy classes for handling groups of events
/*
template< typename KEYBOARD_EVENTS = detail::keyboard_events
class sdl_window : public sdl_window_base
                 , public detail::sdl_timer_base

*/

// active object for displaying images
class sdl_window : public sdl_window_base
                 , public detail::sdl_timer_base
{
public:

   sdl_window( int width
             , int height
             , redraw_event_base*   painter          = NULL
             , keyboard_event_base* keyboard_handler = NULL
             , timer_event_base*    timer_handler    = NULL )
   : sdl_timer_base( timer_handler )
   , _cancel( false )
   , _width  ( width  )
   , _height ( height )
   , _painter( painter )
   , _keyboard_handler( keyboard_handler )
   , _timer_handler( timer_handler )
   , _queue( NULL )
   {
      _screen = SDL_SetVideoMode( width
                                , height
                                , 32
                                , SDL_SWSURFACE );

      if( _screen == NULL )
      {
         throw std::runtime_error( "Couldn't create SDL window" );
      }


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

   void set_queue( queue_t& queue )
   {
      _queue = &queue;
   }

   void _run()
   {
      boost::shared_ptr< sdl_event_t > e;

      while( _cancel == false )
      {
         if( _queue )
         {
            _queue->dequeue( e );

            switch( e->type() )
            {
               case detail::redraw_event::type:
               {
                  lock( _screen );

                  if( _painter )
                  {
                     _painter->redraw( wrap_sdl_image( _screen ));
                  }

                  unlock( _screen
                        , _width
                        , _height  );

                  break;
               }

               case detail::key_up_event::type:
               {
                  lock( _screen );

                  if( _keyboard_handler )
                  {
                     _keyboard_handler->key_up( wrap_sdl_image( _screen ));
                  }

                  unlock( _screen
                        , _width
                        , _height  );

                  break;
               }

               case detail::quit_event::type:
               {
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

   SDL_Surface* _screen;

   int _width;
   int _height;

   // How about shared pointers?
   redraw_event_base*   _painter;
   keyboard_event_base* _keyboard_handler;
   timer_event_base*    _timer_handler;

   queue_t* _queue;

   friend class sdl_service;
};


} } } // namespace boost::gil::sdl

#endif // SDL_WINDOW_HPP
