#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

#include <iostream>
#include <exception>
#include <string>

#include <math.h>

#include <boost/shared_ptr.hpp>

#include <boost/bind.hpp>

#include <boost/scoped_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>

#include "message_queue.h"

struct keyboard_event_base
{
   virtual void key_up( SDL_Surface* screen ) =0;
};

struct painter_base
{
   virtual void render( SDL_Surface* screen ) =0;

protected:

   void set_pixel( const boost::gil::rgb8_pixel_t& pixel
                 , SDL_Surface*        screen
                 , int                 x
                 , int                 y       )
   {
      Uint32 col = SDL_MapRGB( screen->format
                             , get_color( pixel, boost::gil::red_t() )
                             , get_color( pixel, boost::gil::green_t() )
                             , get_color( pixel, boost::gil::blue_t() ));

      char* pos = static_cast<char*>( screen->pixels );

      pos += screen->pitch * y;
      pos += screen->format->BytesPerPixel * x;
      
      memcpy( pos, &col, screen->format->BytesPerPixel );
   }
};

struct event_base
{
   unsigned int type() { return _type; }

protected:

   event_base() : _type( 0 ) {}
   event_base( int type ) : _type( type ) {}

private:

   int _type;
};

struct redraw_event : public event_base
{
   redraw_event()
   : event_base( type )
   {}

   static const int type = 1;
};

struct key_up_event : public event_base
{
   key_up_event()
   : event_base( type )
   {}

   static const int type = 2;
};

struct quit_event : public event_base
{
   quit_event()
   : event_base( type )
   {}

   static const int type = 3;
};

typedef event_base event_t;
typedef ogx::message_queue< event_t > queue_t;


// active object for displaying images
class sdl_window
{
public:

   sdl_window( int width
             , int height
             , painter_base*        painter
             , keyboard_event_base* keyboard_handler )
   : _cancel( false )
   , _width  ( width  )
   , _height ( height )
   , _painter( painter )
   , _keyboard_handler( keyboard_handler )
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
      boost::shared_ptr< event_t > e;

      while( _cancel == false )
      {
         if( _queue )
         {
            _queue->dequeue( e );

            switch( e->type() )
            {
               case redraw_event::type:
               {
                  _render();

                  break;
               }

               case key_up_event::type:
               {
                  _key_up();

                  break;
               }

               case quit_event::type:
               {
                  return;
               }
            }
         }
      }
   }

   void _key_up()
   {
      if( SDL_MUSTLOCK( _screen ))
      {
         if( SDL_LockSurface( _screen ) < 0 ) 
         {
            return;
         }
      }

      if( _keyboard_handler )
      {
         _keyboard_handler->key_up( _screen );
      }

      // Unlock if needed
      if( SDL_MUSTLOCK( _screen )) 
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

   void _render()
   {
      // Lock surface if needed
      if( SDL_MUSTLOCK( _screen ))
      {
         if( SDL_LockSurface( _screen ) < 0 ) 
         {
            return;
         }
      }

      if( _painter )
      {
         _painter->render( _screen );
      }

      // Unlock if needed
      if( SDL_MUSTLOCK( _screen )) 
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

private:

   typedef boost::scoped_ptr<boost::thread> thread_t;
   thread_t _thread;

   mutable boost::mutex _sentinel;

   bool _cancel;

   SDL_Surface* _screen;

   int _width;
   int _height;

   painter_base*        _painter;
   keyboard_event_base* _keyboard_handler;

   queue_t* _queue;

   friend class sdl_service;
};


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
                     _queue.enqueue( boost::shared_ptr< quit_event >( new quit_event() ));

                     return ;
                  }

         			_queue.enqueue( boost::shared_ptr< key_up_event >( new key_up_event() ));

                  break;
               }

               case SDL_QUIT:
               {
                  _queue.enqueue( boost::shared_ptr< quit_event >( new quit_event() ));

                  return ;
               }

            } //switch
         } // while
      } // while
   }

private:

   bool _terminate;

   sdl_window* _win;

   queue_t _queue;
};



#endif //SDL_WRAPPER_HPP