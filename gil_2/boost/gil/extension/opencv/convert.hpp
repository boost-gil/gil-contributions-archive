#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil {
namespace opencv {

void conver_abs_( const ipl_image_wrapper& src
               , ipl_image_wrapper&       dst
               , size_t                   scale = 1
               , size_t                   shift = 0 )
{
   cvConvertScaleAbs( src.get()
                    , dst.get()
                    , static_cast<int>( scale )
                    , static_cast<int>( shift ));
}

template< class SRC_VIEW
        , class DST_VIEW
        >
void conver_abs( SRC_VIEW& src
               , DST_VIEW& dst
               , size_t   scale = 1
               , size_t   shift = 0 )
{
   BOOST_STATIC_ASSERT(( boost::is_same< color_space_type<SRC_VIEW>::type
                                       , color_space_type<DST_VIEW>::type >::type::value ));

   conver_abs_( create_ipl_image( src )
              , create_ipl_image( dst )
              , scale
              , shift    );
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // CONVERT_HPP
