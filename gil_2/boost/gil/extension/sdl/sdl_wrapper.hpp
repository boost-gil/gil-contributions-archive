#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

#include <exception>
#include <string>

#include <math.h>

#include <boost/bind.hpp>

#include <boost/scoped_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>

// active object for displaying images
class sdl_window
{
public:

   sdl_window( int width, int height )
   : _cancel( false )
   , _width ( width  )
   , _height( height )
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
         // Render stuff
         _render();

         // Poll for events, and handle the ones we care about.
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

      // Ask SDL for the time in milliseconds
      int tick = SDL_GetTicks();

      // Declare a couple of variables
      int i, j, yofs, ofs;

      // Draw to screen
      yofs = 0;
      for (i = 0; i < _height; i++)
      {
         for (j = 0, ofs = yofs; j < _width; j++, ofs++)
         {
            ((unsigned int*) _screen->pixels)[ofs] = i * j + i * j + tick;
         }

         yofs += _screen->pitch / 4;
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
};


class sdl_wrapper
{
public:

   static sdl_wrapper& get()
   {
      static sdl_wrapper s;
      return s;
   }

   static void init_sdl()
   {
      // Initialize SDL's subsystems - in this case, only video.
      if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
      {
         std::string error( "Unable to init SDL: " );
         error += SDL_GetError();
         throw  std::runtime_error( error );
      }
   }

private:

   sdl_wrapper() {}
   ~sdl_wrapper() { SDL_Quit(); }
   sdl_wrapper( const sdl_wrapper& ) {}
   sdl_wrapper& operator= ( const sdl_wrapper& ) {}

};


#endif //SDL_WRAPPER_HPP