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

#include <boost/type_traits/is_base_of.hpp>

#include <boost/gil/utilities.hpp>

namespace boost { namespace gil {

typedef boost::gil::point2< std::ptrdiff_t > point_t;

namespace detail {

// - performance specialization double
// - to eliminate compiler warning 4244
template <typename GrayChannelValue>
struct rgb_to_luminance_fn< double, double, double, GrayChannelValue > {
    GrayChannelValue operator()( const double& red
                               , const double& green
                               , const double& blue    ) const
   {
      return channel_convert<GrayChannelValue>( red * 0.30 + green * 0.59 + blue * 0.11 );
   }
};


typedef bit_aligned_image1_type< 1, gray_layout_t >::type gray1_image_t;
typedef bit_aligned_image1_type< 2, gray_layout_t >::type gray2_image_t;
typedef bit_aligned_image1_type< 4, gray_layout_t >::type gray4_image_t;


template < typename Channel >
struct bits_per_sample : public mpl::int_< sizeof( Channel )* 8 / byte_to_memunit< Channel >::value > {};

inline void io_error( const std::string& descr )
{
   throw std::ios_base::failure( descr );
}

inline void io_error_if( bool expr, const std::string& descr )
{
   if( expr ) 
      io_error( descr );
}

template< typename View >
struct is_bit_aligned_impl : public boost::mpl::bool_< byte_to_memunit< typename View::x_iterator >::value == 8 >
{};

template< typename View >
struct is_bit_aligned
{
   static const value = is_base_of< boost::mpl::true_, is_bit_aligned_impl< View > >::value;

   typedef boost::mpl::bool_< boost::is_base_of< boost::mpl::true_
                                               , is_bit_aligned_impl< View > >::value > type;
};

std::string convert_to_string( const std::wstring& s )
{
	std::size_t len = wcslen( s.c_str() );
	char* c = reinterpret_cast<char*>( alloca( len ));
	wcstombs( c, s.c_str(), len );

   return std::string( c, c + len );
}

inline unsigned char swap_bits( unsigned char c )
{
   unsigned char result = 0;
   for( int i = 0; i < 8; ++i )
   {
      result = result << 1;
      result |= ( c & 1 );
      c = c >> 1;
   }

   return result;
}

} // namespace details
} // namespace gil
} // namespace boost

#endif // BASE_HPP
