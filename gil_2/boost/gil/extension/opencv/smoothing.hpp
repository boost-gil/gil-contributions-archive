#ifndef SMOOTHING_HPP
#define SMOOTHING_HPP

#include "ipl_image_wrapper.hpp"

namespace boost { namespace gil { namespace opencv {

struct blur_no_scale{};
struct blur{};
struct gaussian{};
struct median{};
struct bilateral{};

typedef boost::mpl::map<
  boost::mpl::pair< blur_no_scale, boost::mpl::int_< CV_BLUR_NO_SCALE > >
, boost::mpl::pair< blur         , boost::mpl::int_< CV_BLUR          > >
, boost::mpl::pair< gaussian     , boost::mpl::int_< CV_GAUSSIAN      > > 
, boost::mpl::pair< median	      , boost::mpl::int_< CV_MEDIAN        > > 
, boost::mpl::pair< bilateral	   , boost::mpl::int_< CV_BILATERAL     > > 
    > smooth_type_map;


template< class SMOOTH_TYPE >
void smooth( const ipl_image_wrapper& src
           , ipl_image_wrapper&       dst
           , SMOOTH_TYPE&
           , size_t                   param1 = 3
           , size_t                   param2 = 1
           , size_t                   param3 = 1
           , size_t                   param4 = 1 )
           
{
   cvSmooth( src.get()
           , dst.get()
           , static_cast<int>( param1 )
           , static_cast<int>( param2 )
           , static_cast<int>( param3 )
           , static_cast<int>( param4 )
           , boost::mpl::at< smooth_type_map
                           , SMOOTH_TYPE >::type::value  );
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // SMOOTHING_HPP
