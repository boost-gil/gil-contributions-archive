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

struct redraw_handler
{
    void operator() ( rgba8_view_t view )
    {
    }
};

class window
{
public:

    typedef rgba8_image_t image_t;
    typedef image_t::view_t view_t;

    typedef SDL_Event event_t;

public:

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

        // create texture
        _texture = texture_ptr_t( SDL_CreateTexture( _renderer.get()
                                                   , SDL_PIXELFORMAT_RGBA8888
                                                   , SDL_TEXTUREACCESS_STREAMING
                                                   , window_width
                                                   , window_height
                                                   )
                                , SDL_DestroyTexture
                                );

        if( _texture == NULL )
        {
            set_error( true );
            return;
        }

        // create image
        _image = image_t( window_width
                        , window_height
                        );

        // create message queue
        _queue = boost::make_shared< queue_t >();

        // create event loop
        _thread = boost::thread( &window::_run, this );

        //
        _refresh_thread = boost::thread( &window::_refresh, this );
    }

    // Destructor
    ~window()
    {
        set_cancel( true );

        _thread.join();
        _refresh_thread.join();
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

    // Adds an event into window's message queue.
    void add_event( const event_t& e )
    {
        _queue->push( e );
    }

    void set_cancel( const bool cancel ) { lock_t l( _mutex ); _cancel = cancel; }
    bool get_cancel()              const { lock_t l( _mutex );   return _cancel; }

    void set_error( const bool error ) { lock_t l( _mutex ); _error = error; }
    bool get_error() const             { lock_t l( _mutex );  return _error; }

private:

    // Window's message queue.
    void _run()
    {
        event_t e;
            
        while( get_cancel() == false )
        {
            _queue->wait_and_pop( e );

            switch( e.type )
            {
                case SDL_WINDOWEVENT:
                {
                    switch ( e.window.event)
                    {
                        case SDL_WINDOWEVENT_SHOWN:
                        {
                            SDL_Log("Window %d shown", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_HIDDEN:
                        {
                            SDL_Log("Window %d hidden", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_EXPOSED:
                        {
                            SDL_Log("Window %d exposed", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_MOVED:
                        {
                            SDL_Log("Window %d moved to %d,%d",
                                    e.window.windowID, e.window.data1,
                                    e.window.data2);
                            break;
                        }

                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            SDL_Log("Window %d resized to %dx%d",
                                    e.window.windowID, e.window.data1,
                                    e.window.data2);
                            break;
                        }

                        case SDL_WINDOWEVENT_MINIMIZED:
                        {
                            SDL_Log("Window %d minimized", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_MAXIMIZED:
                        {
                            SDL_Log("Window %d maximized", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_RESTORED:
                        {
                            SDL_Log("Window %d restored", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_ENTER:
                        {
                            SDL_Log("Mouse entered window %d",
                                    e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_LEAVE:
                        {
                            SDL_Log("Mouse left window %d", e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                        {
                            SDL_Log("Window %d gained keyboard focus",
                                    e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_LOST:
                        {
                            SDL_Log("Window %d lost keyboard focus",
                                    e.window.windowID);
                            break;
                        }

                        case SDL_WINDOWEVENT_CLOSE:
                        {
                            SDL_Log("Window %d closed", e.window.windowID);
                            break;
                        }

                        default:
                        {
                            SDL_Log("Window %d got unknown event %d",
                                    e.window.windowID, e.window.event);
                            break;
                        }

                    }

                    break;
                }

                case SDL_QUIT:
                {
                    set_cancel( true );

                    break;
                }

                case SDL_KEYDOWN:
                {
                    break;
                }

                default:
                {
                }
            } // switch


        } // while
    }

    // Refresh thread
    void _refresh()
    {
        redraw_handler rh;

        while(  get_cancel() == false 
             ||  get_error() == true
             )
        {
            view_t v = view( _image );

            rh(v);

            fill_pixels( v, view_t::value_type( 0, 0, 255, 255 ));

            SDL_UpdateTexture( _texture.get()
                             , NULL
                             , interleaved_view_get_raw_data( v )
                             , num_channels< view_t >::value * v.width()
                             );

            SDL_RenderClear( _renderer.get() );
    
            SDL_RenderCopy( _renderer.get(), _texture.get(), NULL, NULL );

            SDL_RenderPresent( _renderer.get() );

            boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
        }
    }

private:

    typedef boost::lock_guard< boost::mutex > lock_t;

    typedef SDL_Window window_t;
    typedef boost::shared_ptr< window_t > window_ptr_t;

    typedef SDL_Renderer renderer_t;
    typedef boost::shared_ptr< renderer_t > renderer_ptr_t;

    typedef SDL_Texture texture_t;
    typedef boost::shared_ptr< texture_t > texture_ptr_t;

    typedef boost::shared_ptr< boost::mutex > mutex_ptr_t;

    typedef threadsafe_queue< event_t > queue_t;
    typedef boost::shared_ptr< queue_t > queue_ptr_t;

    window_ptr_t   _window;
    renderer_ptr_t _renderer;
    texture_ptr_t  _texture;

    image_t _image;

    queue_ptr_t _queue;

    boost::thread _thread;
    mutable boost::mutex _mutex;

    boost::thread _refresh_thread;

    bool _error;
    bool _cancel;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_WINDOWS_HPP