/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef BASE_HPP
#define BASE_HPP

/// \file
/// \brief  Defining some basic data types.
//
/// \author Christian Henning
///         

#include <boost/gil/utilities.hpp>

namespace boost { namespace gil {

typedef boost::gil::point2< std::ptrdiff_t > point_t;

namespace detail {

inline void io_error(const char* descr)
{
   throw std::ios_base::failure( descr );
}

inline void io_error_if( bool expr, const char* descr="" )
{
   if( expr ) 
      io_error( descr );
}


template < typename View
         , typename IsPlanar > struct num_planes
{};

template <typename View > struct num_planes< View, boost::mpl::false_ >
{
   typedef boost::mpl::int_<1> type;
};

template <typename View > struct num_planes< View, boost::mpl::true_  > 
{
   typedef typename View::value_type pixel_t;
   typedef boost::mpl::int_< num_channels<pixel_t>::value > type;
};


} // namespace details
} // namespace gil
} // namespace boost

#endif // BASE_HPP
