#ifndef EDGE_DETECTION_HPP
#define EDGE_DETECTION_HPP

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil {
namespace opencv {

void sobel( const ipl_image_wrapper& src
          , ipl_image_wrapper&       dst
          , size_t                   x_order       = 1
          , size_t                   y_order       = 0
          , size_t                   aperture_size = 3 )
{
   cvSobel( src.get()
          , dst.get()
          , static_cast<int>( x_order       )
          , static_cast<int>( y_order       )
          , static_cast<int>( aperture_size ));
}

void laplace( const ipl_image_wrapper& src
            , ipl_image_wrapper&       dst
            , size_t                   aperture_size = 3 )
{
   cvLaplace( src.get()
            , dst.get()
            , static_cast<int>( aperture_size ));
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // EDGE_DETECTION_HPP
