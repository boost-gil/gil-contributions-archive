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

inline 
void io_error( const std::string& descr )
{
   throw std::ios_base::failure( descr );
}

inline 
void io_error_if( bool expr, const std::string& descr )
{
   if( expr ) 
      io_error( descr );
}

template< typename View >
struct is_bit_aligned : public mpl::bool_< byte_to_memunit< typename View::x_iterator >::value == 8 > {};

inline
std::string convert_to_string( const std::wstring& s )
{
	std::size_t len = wcslen( s.c_str() );
	char* c = reinterpret_cast<char*>( alloca( len ));
	wcstombs( c, s.c_str(), len );

   return std::string( c, c + len );
}

inline
unsigned char swap_bits( unsigned char c )
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

template< typename is_bit_aligned
        , typename View
        >
struct read_helper_for_compatible_views
{
   typedef typename View::value_type element_t;
   typedef std::vector< element_t > buffer_t;
   typedef typename buffer_t::const_iterator iterator_t;

   static iterator_t begin( const buffer_t& buffer )
   {
      return iterator_t( buffer.begin() );
   }
                  
   static iterator_t end( const buffer_t& buffer )
   {
      return iterator_t( buffer.end() );
   }
};

template< typename View >
struct read_helper_for_compatible_views< mpl::true_, View >
{
   typedef unsigned char element_t;
   typedef std::vector< element_t > buffer_t;
   typedef typename bit_aligned_pixel_iterator< typename View::reference > iterator_t;

   static iterator_t begin( buffer_t& buffer )
   {
      return iterator_t( &buffer.front(), 0 );
   }
                  
   static iterator_t end( buffer_t& buffer )
   {
      // @todo Won't this break if 8 is not divisible by the bit depth? 
      // What if your bitdepth is 3. The last pixel may not have offset 0
      return iterator_t( &buffer.back() + 1, 0 );
   }
};

} // namespace details
} // namespace gil
} // namespace boost

#endif // BASE_HPP
