#ifndef BOOST_GIL_SDL_BASE_HPP
#define BOOST_GIL_SDL_BASE_HPP

#include <SDL.h>

#include <boost/shared_ptr.hpp>

#include <boost/gil/extension/sdl2/threadsafe_queue.hpp>


typedef SDL_Event event_t;

typedef threadsafe_queue< event_t > queue_t;
typedef boost::shared_ptr< queue_t > queue_ptr_t;


#endif // BOOST_GIL_SDL_BASE_HPP
