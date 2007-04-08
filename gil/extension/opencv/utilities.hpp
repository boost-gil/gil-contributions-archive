#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cv.h>

#include <boost/shared_array.hpp>

#include <gil/core/gil_all.hpp>

namespace opencv {

typedef gil::point2<ptrdiff_t> point_t;
typedef std::vector< point_t > curve_t;
typedef std::vector< curve_t > curve_vec_t;

typedef boost::shared_array<CvPoint> cvpoint_array_t;
typedef std::vector< cvpoint_array_t > cvpoint_array_vec_t;

inline
CvPoint make_cvPoint( point_t point )
{
   return cvPoint( point.x, point.y );
}

inline
cvpoint_array_t make_cvPoint_array( const curve_t& curve )
{
   std::size_t curve_size = curve.size();

   cvpoint_array_t cvpoint_array( new CvPoint[ curve.size() ] );

   for( std::size_t i = 0; i < curve_size ; ++i )
   {
      cvpoint_array[i] = make_cvPoint( curve[i] );
   }

   return cvpoint_array;
}

inline
CvSize make_cvSize( point_t point )
{
   return cvSize( point.x, point.y );
}

template< class PIXEL >
inline
CvScalar make_cvScalar( const PIXEL& pixel )
{
   CvScalar s;
   for( int i = 0; i < PIXEL::num_channels; ++i )
   {
      s.val[i] = pixel[i];
   }

   return s;
}

} // namespace opencv


#endif // UTILITIES_HPP