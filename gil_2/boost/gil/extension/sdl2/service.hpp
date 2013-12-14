/*
    Copyright 2013 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_SDL_SERVICE_HPP
#define BOOST_GIL_SDL_SERVICE_HPP

#include <SDL.h>

#include <unordered_map>

#include <boost/noncopyable.hpp>

#include <boost/thread/once.hpp>

#include <boost/gil/extension/sdl2/base.hpp>
#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

namespace boost { namespace gil { namespace sdl {

class service : boost::noncopyable
{

public:

    service( boost::uint32_t flag = SDL_INIT_EVERYTHING )
    : _initializer( flag )
    {}

    template< typename Window >
    void add_window( Window& w )
    {
        _windows[w.get_id()] = &w;
    }

    void run()
    {
        bool quit = false;
        SDL_Event e;

        while( quit == false )
        {
            // we need to use SDL_PollEvent since the windows will
            // shut down asynchronously.
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

                    default: 
                    {
                        add_event( e );

                        break;
                    }
                } // switch
            }

            if( done() )
            {
                quit = true;
                break;
            }
        }
    }

private:

    void add_event( const event_t& e )
    {
        std::for_each( _windows.begin(), _windows.end()
                     , [&]( window_map_t::reference w ) 
                        {
                            if( w.second )
                            {
                                w.second->add_event( e );
                            }
                        }
                     );
    }

    void add_event( const event_t& e, int id )
    {
        window_base* w = _windows[id];
        
        if( w )
        {
            w->add_event( e );
        }
    }

    // Loop through all windows and if all are done return true.
    bool done()
    {
        bool quit = true;

        std::for_each( _windows.begin(), _windows.end()
                     , [&]( window_map_t::reference w ) 
                        {
                            if( w.second )
                            {
                                if( w.second->get_cancel() == false )
                                {
                                    quit = false;
                                }
                            }
                        }
                     );

        return quit;
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

    // Sleep the event receiving thread for some time. 
    unsigned int _delay;
    
    initializer _initializer;

    typedef std::unordered_map< int, window_base* > window_map_t;
    window_map_t _windows;
};

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_SERVICE_HPP
