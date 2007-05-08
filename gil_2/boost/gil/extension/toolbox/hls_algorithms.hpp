#ifndef HSL_ALGORITHMS_HPP
#define HSL_ALGORITHMS_HPP

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {
namespace toolbox {

template< typename HSL_VIEW >
void shift_hue( const HSL_VIEW& src
              , const HSL_VIEW& dst
              , const bits32f   value )
{
   // make sure src and dst are views
   // make sure src and dst are either hsl or hsv

   typedef HSL_VIEW::value_type pixel_t;

   typedef channel_type<pixel_t>::type channel_t;

   typedef channel_t& (*AA)( pixel_t& );
   typedef void (*BB)( channel_t& );

   for_each( view( hsv_img ).begin()
           , view( hsv_img ).end()
           , bind( static_cast<BB>( &set_to_orange )
                 , bind( static_cast<AA>( &get_channel_0<channel_t> ), _1 )));
}


} //namespace toolbox
} //namespace gil
} //namespace boost

#endif // HSL_ALGORITHMS_HPP