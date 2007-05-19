// Copyright 2007 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef SDL_SERVICE_HPP
#define SDL_SERVICE_HPP

#include <exception>
#include <string>

#include <boost/scoped_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>

#include "sdl_window.hpp"
#include "sdl_events.hpp"
#include "message_queue.h"

namespace boost { namespace gil { namespace sdl { 

class sdl_service
{
public:

   sdl_service()
   : _terminate( false )
   {
      if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
      {
         std::string error( "Unable to init SDL: " );
         error += SDL_GetError();
         throw  std::runtime_error( error );
      }
   }

   ~sdl_service()
   {
      _terminate = true;

      SDL_Quit();
   }

   void add_window( sdl_window& win )
   {
      _win = &win;

      // Pass the window a message queue.
      
      _win->set_queue( _queue );
   }

   void run()
   {
      while( _terminate == false )
      {
         SDL_Event event;
         while( SDL_PollEvent( &event ))
         {
            add_message<detail::redraw_event>();

            switch (event.type) 
            {
               case SDL_KEYDOWN:
               {
                  break;
               }

               case SDL_KEYUP:
               {
                  // If escape is pressed, return (and thus, quit)
                  if( event.key.keysym.sym == SDLK_ESCAPE )
                  {
                     add_message<detail::quit_event>();

                     return ;
                  }

                  add_message<detail::key_up_event>();

                  break;
               }

               case SDL_QUIT:
               {
                  add_message<detail::quit_event>();

                  return ;
               }

            } //switch
         } // while
      } // while
   }

private:

   template< class EVENT >
   void add_message()
   {
      _queue.enqueue( boost::shared_ptr< EVENT >( new EVENT() ));
   }

   bool _terminate;

   sdl_window* _win;

   queue_t _queue;
};


} } } // namespace boost::gil::sdl

#endif // SDL_SERVICE_HPP
