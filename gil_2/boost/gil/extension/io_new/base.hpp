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
struct is_bit_aligned
{
   typedef boost::mpl::false_ type;
   static const bool value = false;
};

template<>
struct is_bit_aligned< bit_aligned_image1_type<1, gray_layout_t>::type::view_t >
{
   typedef boost::mpl::true_ type;
   static const bool value = true;
};

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

std::string convert_to_string( const std::wstring& s )
{
	std::size_t len = wcslen( s.c_str() );
	char* c = reinterpret_cast<char*>( alloca( len ));
	wcstombs( c, s.c_str(), len );

   return std::string( c, c + len );
}

} // namespace details
} // namespace gil
} // namespace boost

#endif // BASE_HPP
