#ifndef EDGE_DETECTION_HPP
#define EDGE_DETECTION_HPP

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil { namespace opencv {

void sobel( const ipl_image_wrapper& src
          , ipl_image_wrapper&       dst
          , size_t                   x_order       = 1
          , size_t                   y_order       = 0
          , size_t                   aperture_size = 3 )
{
   cvSobel( src.get()
          , dst.get()
          , x_order
          , y_order
          , aperture_size
          );
}

template< typename View >
void sobel( View        src
          , View        dst
          , std::size_t x_order       = 1
          , std::size_t y_order       = 0
          , std::size_t aperture_size = 3 )
{
    ipl_image_wrapper src_ipl = create_ipl_image( src );
    ipl_image_wrapper dst_ipl = create_ipl_image( dst );

    sobel( src_ipl
         , dst_ipl
         , x_order
         , y_order
         , aperture_size
         );
}

void laplace( const ipl_image_wrapper& src
            , ipl_image_wrapper&       dst
            , size_t                   aperture_size = 3 )
{
   cvLaplace( src.get()
            , dst.get()
            , aperture_size
            );
}

template< typename View_Src
        , typename View_Dst
        >
void laplace( View_Src    src
            , View_Dst    dst
            , std::size_t aperture_size = 3 )
{
    ipl_image_wrapper src_ipl = create_ipl_image( src );
    ipl_image_wrapper dst_ipl = create_ipl_image( dst );

    laplace( src_ipl
           , dst_ipl
           , aperture_size
           );
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // EDGE_DETECTION_HPP
