#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil {
namespace opencv {

struct nearest_neigbor{};
struct bilinear{};
struct area{};
struct bicubic{};

typedef boost::mpl::map<
  boost::mpl::pair< nearest_neigbor, boost::mpl::int_<CV_INTER_NN     > >
, boost::mpl::pair< bilinear       , boost::mpl::int_<CV_INTER_LINEAR > >
, boost::mpl::pair< area           , boost::mpl::int_<CV_INTER_AREA   > > 
, boost::mpl::pair< bicubic	     , boost::mpl::int_<CV_INTER_CUBIC  > > 
    > interpolation_method_map;


template< class INTERPOLATION >
void resize( const ipl_image_wrapper& src
           , ipl_image_wrapper&       dst
           , INTERPOLATION            )
{
   cvResize( src.get()
           , dst.get()
           , boost::mpl::at< interpolation_method_map
                           , INTERPOLATION >::type::value  );
}


} // namespace opencv
} // namespace gil
} // namespace boost

#endif // GIL_RESIZE_H
