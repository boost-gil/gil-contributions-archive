#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <math.h>

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
         throw runtime_error( "Couldn't create SDL window" );
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
      boost::mutex::scoped_lock oLock( _oSentinel );

      _cancel = true;
   }

private:

   void run()
   {
      while( _cancel == false )
      {
         // Render stuff
         render();

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
                     return 0;

                  break;
               }

               case SDL_QUIT:
               {
                  return(0);
               }

            } //switch
         } // while
      } // while
   }

   void render()
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
      for (i = 0; i < 768; i++)
      {
         for (j = 0, ofs = yofs; j < 1024; j++, ofs++)
         {
            ((unsigned int*)screen->pixels)[ofs] = i * j + i * j + tick;
         }

         yofs += screen->pitch / 4;
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
         string error( "Unable to init SDL: " );
         error += SDL_GetError();
         throw  runtime_error( error )
      }

      // Register SDL_Quit to be called at exit; makes sure things are
      // cleaned up when we quit.
      atexit(SDL_Quit);
   }

private:

   sdl_wrapper();
   sdl_wrapper( const sdl_wrapper& );
   sdl_wrapper& operator= ( const sdl_wrapper& );

};


#endif //SDL_WRAPPER_HPP