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

#include <boost/gil/extension/sdl2/base.hpp>
#include <boost/gil/extension/sdl2/default_event_handlers.hpp>

namespace boost { namespace gil { namespace sdl {

struct sdl_error : std::exception
{
    const char* what() const throw()
    {
        return "SDL subsystem error.";
    }
};

//
// A base class for windows which can be passed around to access
// some information. No template arguments should be needed here.
//
class window_base
{
public:

    window_base()
    : _error( false )
    , _cancel( false )
    {}

    bool get_cancel() const { lock_t l( _mutex ); return _cancel; }
    bool get_error()  const { lock_t l( _mutex ); return _error; }

    // Adds an event into window's message queue.
    void add_event( const event_t& e )
    {
        _queue->push( e );
    }

protected:

    // these function will be called from the two window threads ( event loop and redraw ).
    void set_cancel( const bool cancel ) { _cancel = cancel; }
    void set_error( const bool error ) { _error = error; }

protected:

    typedef boost::lock_guard< boost::mutex > lock_t;

    queue_ptr_t _queue;

    mutable boost::mutex _mutex;

    bool _error;
    bool _cancel;
};


//
// Window class which wraps SDL's window specifics.
//
// Despite the SDL specifics each window has a message queue to receive
// events. A seperate thread will serve as a event loop.
//
// Another thread encapsulates the redrawing functionality. This thread
// will run at a different speed than the event loop. Ideally the redrsw 
// handler only copies the next image into the window's texture.
// 
template< typename Redraw_Handler         = default_redraw_handler
        , typename Keyboard_Event_Handler = default_keyboard_event_handler
        >
class window : public window_base
{
public:

    typedef window_base base_t;
    typedef window< Redraw_Handler, Keyboard_Event_Handler > this_t;

    typedef rgba8_image_t image_t;
    typedef image_t::view_t view_t;

public:

    // Constructor
    window( const unsigned int    fps = 30
          , const char*           title = NULL
          , const int             window_pos_x = SDL_WINDOWPOS_CENTERED
          , const int             window_pos_y = SDL_WINDOWPOS_CENTERED
          , const int             window_width = 640
          , const int             window_height = 480
          , const boost::uint32_t window_flags = SDL_WINDOW_SHOWN
          , const int             renderer_index = -1
          , const boost::uint32_t renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
          )
    : window_base()
    , _fps( fps )
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
                                                   , SDL_PIXELFORMAT_ABGR8888
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
        _event_loop = boost::thread( &window::_run, this );

        //
        _redraw_thread = boost::thread( &window::_redraw, this );
    }

    // Destructor
    // Will be called when SDL service finishes.
    ~window()
    {
        set_cancel( true );

        _event_loop.join();
        _redraw_thread.join();
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


    void         set_fps( const unsigned int fps ) { lock_t l( _mutex ); _fps = fps; }
    unsigned int get_fps() const                   { lock_t l( _mutex );  return _fps; }

    queue_ptr_t get_queue() const { return _queue; }

private:

    // Window's message queue.
    void _run()
    {
        Keyboard_Event_Handler keh;

        event_t e;
            
        while( get_cancel() == false && get_error() == false )
        {
            _queue->wait_and_pop( e );

            lock_t l( _mutex );

            switch( e.type )
            {
                case SDL_WINDOWEVENT:
                {
                    break;
                }

                case SDL_QUIT:
                {
                    set_cancel( true );

                    break;
                }

                case SDL_KEYDOWN:
                {
                    keh(*this, e);

                    break;
                }
            } // switch

        } // while

        _close();
    }

    // Refresh thread
    void _redraw()
    {
        Redraw_Handler rh;

        while(  get_cancel() == false 
             ||  get_error() == true
             )
        {
            {
                lock_t l( _mutex );

                view_t v = view( _image );

                rh.operator()< view_t >( v );

                SDL_UpdateTexture( _texture.get()
                                 , NULL
                                 , interleaved_view_get_raw_data( v )
                                 , num_channels< view_t >::value * v.width()
                                 );

                SDL_RenderClear( _renderer.get() );
    
                SDL_RenderCopy( _renderer.get(), _texture.get(), NULL, NULL );

                SDL_RenderPresent( _renderer.get() );

            }

            //boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 / get_fps() ) );
            boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
        }
    }

    void _close()
    {
        lock_t l( _mutex );

        _texture.reset();
        _renderer.reset();
        
        // resetting a window doesn't work here. So the destructor will have to do.
        //_window.reset();
    }

private:

    typedef SDL_Window window_t;
    typedef boost::shared_ptr< window_t > window_ptr_t;

    typedef SDL_Renderer renderer_t;
    typedef boost::shared_ptr< renderer_t > renderer_ptr_t;

    typedef SDL_Texture texture_t;
    typedef boost::shared_ptr< texture_t > texture_ptr_t;

    window_ptr_t   _window;
    renderer_ptr_t _renderer;
    texture_ptr_t  _texture;

    image_t _image;

    boost::thread _event_loop;
    boost::thread _redraw_thread;

    unsigned int _fps;


    friend Redraw_Handler;
    friend Keyboard_Event_Handler;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_WINDOWS_HPP