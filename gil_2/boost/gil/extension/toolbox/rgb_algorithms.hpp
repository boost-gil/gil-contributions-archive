#ifndef RGB_ALGORITHMS_HPP
#define RGB_ALGORITHMS_HPP

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {
namespace toolbox {

struct calc_negative
{
   template < typename DST_CHANNEL
            , typename SRC_CHANNEL
            > void operator()( DST_CHANNEL& dst 
                             , SRC_CHANNEL& src ) const
   {
      SRC_CHANNEL max = channel_traits< SRC_CHANNEL >::max_value();

      dst = max - src;
   }
};


template< typename RGB_VIEW >
void negative( const RGB_VIEW& src
             , const RGB_VIEW& dst )
{
   // only for rgb views.

   // make sure src and dst have same dimensions.

   // for rgba do it only for rgb channels

   typedef channel_type< RGB_VIEW >::type channel_t;
   channel_t max = channel_traits<channel_t>::max_value();

   for( int y=0; y < src.height(); ++y )
   {
      typename RGB_VIEW::x_iterator src_it = src.row_begin( y );
      typename RGB_VIEW::x_iterator dst_it = dst.row_begin( y );

      for( int x = 0; x < src.width(); ++x )
      {
         static_for_each( dst_it[x], src_it[x], calc_negative() );
      }
   }
}


struct calc_brightness
{
   float _factor;

   template < typename DST_CHANNEL
            , typename SRC_CHANNEL
            > void operator()( DST_CHANNEL&       dst 
                             , const SRC_CHANNEL& src ) const
   {
      float d = static_cast<float>( src ) * _factor;

      if( d > channel_traits< DST_CHANNEL >::max_value() )
      {
         dst = channel_traits< DST_CHANNEL >::max_value();
      }
      else
      {
         dst = static_cast< DST_CHANNEL >( d );
      }
   }
};



template< typename RGB_VIEW >
void brightness( const RGB_VIEW& src
               , const RGB_VIEW& dst
               , float           factor )
{
   calc_brightness calc;
   calc._factor = factor;

   for( int y=0; y < src.height(); ++y )
   {
      typename RGB_VIEW::x_iterator src_it = src.row_begin( y );
      typename RGB_VIEW::x_iterator dst_it = dst.row_begin( y );

      for( int x = 0; x < src.width(); ++x )
      {
         static_for_each( dst_it[x], src_it[x], calc );
      }
   }
}

} //namespace toolbox
} //namespace gil
} //namespace boost

#endif // RGB_ALGORITHMS_HPP