#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

#include <iostream>
#include <exception>
#include <string>

#include <math.h>

#include <boost/bind.hpp>

#include <boost/scoped_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>

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

// active object for displaying images
class sdl_window
{
public:

   sdl_window( int width, int height, painter_base* painter )
   : _cancel( false )
   , _width  ( width  )
   , _height ( height )
   , _painter( painter )
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

   void _run()
   {
      while( _cancel == false )
      {
      }
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

      _painter->render( _screen );

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

   painter_base* _painter;

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
   }

   void run()
   {
      while( _terminate == false )
      {
         // send out render signal.
         _win->_render();

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
                     return ;

                  break;
               }

               case SDL_QUIT:
               {
                  return ;
               }

            } //switch
         } // while
      } // while
   }

private:

   bool _terminate;

   sdl_window* _win;
};



#endif //SDL_WRAPPER_HPP