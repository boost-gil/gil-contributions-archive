#ifndef BOOST_GIL_SDL_WINDOWS_HPP
#define BOOST_GIL_SDL_WINDOWS_HPP

#include <SDL.h>

#include <boost/cstdint.hpp>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>

namespace boost { namespace gil { namespace sdl {

struct sdl_error : std::exception
{
    const char* what() const throw()
    {
        return "SDL subsystem error.";
    }
};

class window
{
public:

    typedef bgra8_view_t view_t;

public:

    static Uint32 draw( Uint32 interval, void* param )
    {
        window* w = (window*) param;

        SDL_Rect viewport;
        SDL_RenderGetViewport( w->_renderer.get(), &viewport );

        SDL_SetRenderDrawColor(w->_renderer.get(), 0, 0, 0, 255);
        SDL_RenderFillRect(w->_renderer.get(), NULL);


        SDL_SetRenderDrawColor( w->_renderer.get(), 255, 255, 0, 255 );

        for( int i = 0; i < 10; ++i )
        {
            SDL_RenderDrawPoint( w->_renderer.get()
                               , rand() % viewport.w
                               , rand() % viewport.h
                               );
        }

        SDL_RenderPresent( w->_renderer.get() );

        return interval;
    }

    // Constructor
    window( const char*           title = NULL
          , const int             window_pos_x = SDL_WINDOWPOS_CENTERED
          , const int             window_pos_y = SDL_WINDOWPOS_CENTERED
          , const int             window_width = 640
          , const int             window_height = 480
          , const boost::uint32_t window_flags = SDL_WINDOW_SHOWN
          , const int             renderer_index = -1
          , const boost::uint32_t renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
          )
    : _error( false )
    , _cancel( false )
    {
        // create window
        _window = window_ptr_t( SDL_CreateWindow( title
                                                , window_pos_x
                                                , window_pos_y
                                                , window_width
                                                , window_height
                                                , window_flags
                                                )

                              , SDL_DestroyWindow
                              );

        if( _window == NULL )
        {
            set_error( true );
            return;
        }

        // create renderer
        _renderer = renderer_ptr_t( SDL_CreateRenderer( _window.get()
                                                      , renderer_index
                                                      , renderer_flags
                                                      )
                                  , SDL_DestroyRenderer
                                  );


        if( _renderer == NULL )
        {
            set_error( true );
            return;
        }

        // create surface
        //_surface = surface_ptr_t( SDL_CreateRGBSurface( 0 // obsolete
        //                                              , window_width
        //                                              , window_height
        //                                              , 32
        //                                              , 0, 0, 0, 0
        //                                              )
        //                        , SDL_FreeSurface
        //                        );

        //if( _surface == NULL )
        //{
        //    set_error( true );
        //    return;
        //}


        // create message queue
        _queue = boost::make_shared< queue_t >();

        // create event loop
        _thread = boost::thread( &window::_run, this );

        SDL_AddTimer( 100, window::draw, this );
    }

    // Destructor
    ~window()
    {
        set_error( true );

        _thread.join();
    }

    int get_id() const 
    {
        if( get_error() ) 
            throw sdl_error();

        int index = SDL_GetWindowID( _window.get() );

        if( index == -1 )
        {
            throw sdl_error();
        }

        return index;
    }

    void add_event( int e )
    {
        _queue->push( e );
    }

    void draw()
    {
        if( _error )
        {
            throw sdl_error();
        }

        texture_ptr_t texture = texture_ptr_t( SDL_CreateTextureFromSurface( _renderer.get()
                                                                           , _surface.get()
                                                                           )
                                             , SDL_DestroyTexture
                                             );

        if( texture == NULL )
        {
            throw sdl_error();
        }
        
        SDL_RenderCopy( _renderer.get()
                      , texture.get()
                      , NULL
                      , NULL
                      );

        SDL_RenderPresent( _renderer.get() );

        boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ));
    }

    bgra8_view_t get_view()
    {
        return interleaved_view( _surface->w
                               , _surface->h
                               , (boost::gil::bgra8_pixel_t*) _surface->pixels
                               , _surface->pitch
                               );
    }


    void set_cancel( const bool cancel ) { lock_t l( _mutex ); _cancel = cancel; }
    bool get_cancel()              const { lock_t l( _mutex );   return _cancel; }

    void set_error( const bool error ) { lock_t l( _mutex ); _error = error; }
    bool get_error() const             { lock_t l( _mutex );  return _error; }

private:

    void _run()
    {
        int e;
            
        while( get_cancel() == false )
        {
            _queue->wait_and_pop( e );

            switch( e )
            {
                case SDL_QUIT:
                {
                    set_cancel( true );

                    break;
                }

                case SDL_KEYDOWN:
                {
                    break;
                }
            }
        }
    }

private:

    typedef boost::lock_guard< boost::mutex > lock_t;

    typedef SDL_Window window_t;
    typedef boost::shared_ptr< window_t > window_ptr_t;

    typedef SDL_Renderer renderer_t;
    typedef boost::shared_ptr< renderer_t > renderer_ptr_t;

    typedef SDL_Surface surface_t;
    typedef boost::shared_ptr< surface_t > surface_ptr_t;

    typedef SDL_Texture texture_t;
    typedef boost::shared_ptr< texture_t > texture_ptr_t;

    typedef boost::shared_ptr< boost::mutex > mutex_ptr_t;

    typedef threadsafe_queue< int > queue_t;
    typedef boost::shared_ptr< queue_t > queue_ptr_t;



    window_ptr_t   _window;
    renderer_ptr_t _renderer;
    surface_ptr_t  _surface;

    queue_ptr_t _queue;

    boost::thread _thread;
    mutable boost::mutex _mutex;

    bool _error;
    bool _cancel;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_WINDOWS_HPP