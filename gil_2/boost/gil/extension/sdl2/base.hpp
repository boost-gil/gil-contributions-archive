/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_SDL_BASE_HPP
#define BOOST_GIL_SDL_BASE_HPP

#include <SDL.h>

#include <boost/shared_ptr.hpp>

#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>

namespace boost { namespace gil { namespace sdl {

typedef SDL_Event event_t;

typedef threadsafe_queue< event_t > queue_t;
typedef boost::shared_ptr< queue_t > queue_ptr_t;

} } } // namespace boost::gil::sdl

#endif // BOOST_GIL_SDL_BASE_HPP
