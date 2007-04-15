#ifndef COLOR_CONVERSION_HPP
#define COLOR_CONVERSION_HPP

#include <cv.h>

#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

#include <gil/core/gil_all.hpp>

namespace boost { namespace gil {
namespace opencv {

typedef boost::mpl::vector< gil::rgb8_pixel_t, gil::xyz_pixel_t > rgb8_xyz_t;
typedef boost::mpl::vector< gil::bgr8_pixel_t, gil::xyz_pixel_t > bgr8_xyz_t;

typedef boost::mpl::map
<
   boost::mpl::pair< rgb8_xyz_t,  boost::mpl::int_< CV_RGB2XYZ > >,
   boost::mpl::pair< bgr8_xyz_t,  boost::mpl::int_< CV_BGR2XYZ > >
> code_types;


template< class SRC
        , class DST >
void convert_color( SRC src, DST dst )
{
   boost::mpl::vector< typename SRC::pixel_t
                     , typename DST::pixel_t > view_types;

   assert( src.dimension() == dst.dimension() );

   cvCvtColor( &src.begin()[0]
             , &dst.begin()[0]
             , boost::mpl::at< code_types, view_types >::type::value;
}

} //namespace opencv
} //namespace gil
} //namespace boost

#endif // COLOR_CONVERSION_HPP