#ifndef BOOST_GIL_SDL_SERVICE_HPP
#define BOOST_GIL_SDL_SERVICE_HPP

#include <SDL.h>

#include <unordered_map>

#include <boost/noncopyable.hpp>

#include <boost/thread/once.hpp>

#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

namespace boost { namespace gil { namespace sdl {

class service : boost::noncopyable
{

public:

    service( boost::uint32_t flag = SDL_INIT_EVERYTHING )
    : _initializer( flag )
    {}

    void add_window( window& w )
    {
        _windows[w.get_id()] = &w;
    }

    void run()
    {
        bool quit = false;
        SDL_Event e;

        while( quit == false )
        {
            while( SDL_PollEvent( &e ))
            {
                switch( e.type )
                {
                    case SDL_WINDOWEVENT:
                    {
                        add_event( e
                                 , e.window.windowID
                                 );

                        break;
                    }

                    case SDL_QUIT:
                    {
                        quit = true;

                        add_event( e );

                        SDL_Log( "quitting" );

                        //boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ));

                        break;
                    }

                    default: 
                    {
                        add_event( e );

                        break;
                    }

                }
            }
        }
    }

private:

    void add_event( const SDL_Event& e )
    {
        std::for_each( _windows.begin(), _windows.end()
                     , [&]( window_map_t::reference w ) 
                        {
                            assert( w.second );

                            if( w.second )
                            {
                                w.second->add_event( e );
                            }
                        }
                     );
    }

    void add_event( const SDL_Event& e, int id )
    {
        auto* w = _windows[id];
        assert( w );
        
        if( w )
        {
            w->add_event( e );
        }
    }

private:

    class initializer
    {
    public:

        // constructor
        initializer( boost::uint32_t flags )
        {
            static boost::once_flag flag = BOOST_ONCE_INIT;
            boost::call_once( flag, [&flags] () { SDL_Init( flags ); } );
        }

        // destructor
        ~initializer()
        {
            static boost::once_flag flag = BOOST_ONCE_INIT;
            boost::call_once( flag, [] () { SDL_Quit(); } );
        }
    };


private:

    initializer _initializer;

    typedef std::unordered_map< int, window* > window_map_t;
    window_map_t _windows;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_SERVICE_HPP
