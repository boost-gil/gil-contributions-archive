/*
    Copyright 2013 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_SDL_DEAULT_EVENT_HANDLERS_HPP
#define BOOST_GIL_SDL_DEAULT_EVENT_HANDLERS_HPP

#include <boost/gil/extension/sdl2/base.hpp>

namespace boost { namespace gil { namespace sdl {

struct default_redraw_handler
{
    void operator() ( rgba8_view_t view )
    {}
};

struct default_keyboard_event_handler
{
    template< typename Window >
    void operator() ( Window& w
                    , event_t e
                    )
    {}
};

struct default_window_event_handler
{
    template< typename Window >
    void operator() ( Window& w
                    , event_t e
                    )
    {}
};



} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_DEAULT_EVENT_HANDLERS_HPP