#ifndef BOOST_GIL_SDL_SERVICE_HPP
#define BOOST_GIL_SDL_SERVICE_HPP

#include <SDL.h>

#include <unordered_map>

#include <boost/thread/once.hpp>

#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

namespace boost { namespace gil { namespace sdl {

class initializer
{
public:

    // constructor
    initializer()
    {
        static boost::once_flag flag = BOOST_ONCE_INIT;
        boost::call_once( flag, [] () { SDL_Init( SDL_INIT_EVERYTHING ); } );
    }

    // destructor
    ~initializer()
    {
        static boost::once_flag flag = BOOST_ONCE_INIT;
        boost::call_once( flag, [] () { SDL_Quit(); } );
    }
};

class service
{

public:

    service()
    : initializer( boost::make_shared< sdl::initializer >() )
    {}

    void add_window( const window& w )
    {
        _windows[w.get_index()] = w;
    }

    void run()
    {
        SDL_Event e;

        while( SDL_PollEvent( &e ))
        {
            switch( e.type )
            {
                case SDL_WINDOWEVENT:
                {
                    add_event( SDL_WINDOWEVENT
                             , e.window.windowID
                             );
                }

                case SDL_QUIT:
                {
                    add_event( SDL_QUIT );

                    break;
                }
            }
        }
    }

private:

    void add_event( int e )
    {
        std::for_each( _windows.begin(), _windows.end()
                , [&]( window_map_t::reference w ) { w.second.add_event( e ); }
                );
    }

    void add_event( int e, int id )
    {
        _windows[id].add_event( e );
    }


private:

    typedef boost::shared_ptr< initializer > initializer_ptr_t;
    initializer_ptr_t initializer;

    typedef std::unordered_map< int, window > window_map_t;
    window_map_t _windows;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_SERVICE_HPP
