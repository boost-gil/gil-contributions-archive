#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil { namespace opencv {

struct nearest_neigbor : boost::mpl::int_< CV_INTER_NN >     {};
struct bilinear        : boost::mpl::int_< CV_INTER_LINEAR > {};
struct area            : boost::mpl::int_< CV_INTER_AREA >   {};
struct bicubic         : boost::mpl::int_< CV_INTER_CUBIC >  {};

template< class Interpolation >
void resize( const ipl_image_wrapper& src
           , ipl_image_wrapper&       dst
           , const Interpolation& 
           )
{
   cvResize( src.get()
           , dst.get()
           , Interpolation::type::value
           );
}

template< typename View
        , typename Interpolation
        >
void resize( View                 src
           , View                 dst
           , const Interpolation& interpolation
           )
{
    ipl_image_wrapper src_ipl = create_ipl_image( src );
    ipl_image_wrapper dst_ipl = create_ipl_image( dst );

    resize( src_ipl
          , dst_ipl
          , interpolation
          );
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // GIL_RESIZE_H
