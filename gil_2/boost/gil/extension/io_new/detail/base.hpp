/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_BASE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_BASE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <istream>
#include <vector>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/gil/utilities.hpp>
#include <boost/gil/color_convert.hpp>
#include <boost/gil/bit_aligned_pixel_reference.hpp>
#include <boost/gil/bit_aligned_pixel_iterator.hpp>

#include "typedefs.hpp"
#include "gil_extensions.hpp"

namespace boost { namespace gil {

typedef unsigned char byte_t;
typedef boost::gil::point2< std::ptrdiff_t > point_t;

struct format_tag {};

template< typename Property > 
struct property_base
{
    typedef Property type;
};

/**
 * Boolean meta function, mpl::true_ if the pixel type \a PixelType is supported 
 * by the image format identified with \a FormatTag.
 * \todo the name is_supported is to generic, pick something more IO realted.
 */
template<typename PixelType, typename FormatTag> struct is_supported;

template<typename FormatTag> struct is_format_tag : is_base_and_derived< format_tag
                                                                       , FormatTag
                                                                       > {};

template<typename FormatTag> struct image_read_info;

struct image_read_settings_base
{
protected:

    image_read_settings_base()
    : _top_left( 0, 0 )
    , _dim     ( 0, 0 )
    {}

    image_read_settings_base( const point_t& top_left
                            , const point_t& dim
                            )
    : _top_left( top_left )
    , _dim     ( dim      )
    {}

public:

    point_t _top_left;
    point_t _dim;
};

template<typename FormatTag> struct image_read_settings;
template<typename FormatTag> struct image_write_info;


namespace detail {

template< typename Property > 
struct property_base
{
    typedef Property type;
};


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

template<typename PixelT,typename DummyT = void >
struct row_buffer_helper
{
    typedef PixelT element_t;
    typedef std::vector< element_t > buffer_t;
    typedef typename buffer_t::iterator iterator_t;

    buffer_t row_buffer;
    row_buffer_helper( int width )
        : row_buffer(width) 
    {
    }

    element_t* data()
    {
        return &row_buffer[0];
    }

    iterator_t begin()
    {
        return row_buffer.begin();
    }

    iterator_t end()
    {
        return row_buffer.end();
    }
};

template<typename PixelT>
struct row_buffer_helper<PixelT, typename enable_if<
    typename mpl::and_<
        typename is_bit_aligned<PixelT>::type,
        typename is_homogeneous<PixelT>::type
        >::type
    >::type>
{
    typedef unsigned char element_t;
    typedef std::vector< element_t > buffer_t;
    typedef PixelT pixel_type;
    typedef bit_aligned_pixel_iterator<pixel_type> iterator_t;


    int c, r;
    buffer_t row_buffer;
    row_buffer_helper( int width )
        : c(
                (width * 
                num_channels<pixel_type>::type::value *
                channel_type<pixel_type>::type::num_bits) >>
                3 
                ),
        r( 
                width * 
                num_channels<pixel_type>::type::value *
                channel_type<pixel_type>::type::num_bits  -
                ( c << 3 )
         ), 
        row_buffer(c + (r!=0))
    {
    }

    iterator_t begin()
    {
        return iterator_t(&row_buffer.front(),0);
    }

    iterator_t end()
    {
        return r==0 
            ? iterator_t(&row_buffer.back() + 1,0)
            : iterator_t(&row_buffer.back(), r);
    }

    element_t* data()
    {
        return &row_buffer[0];
    }
};

template<typename View,typename D = void>
struct row_buffer_helper_view
    : row_buffer_helper<typename View::value_type>
{
    row_buffer_helper_view( int width ) 
      :  row_buffer_helper<typename View::value_type>(width)
    {}
};


template<typename View>
struct row_buffer_helper_view<View,
    typename enable_if<typename is_bit_aligned<typename View::value_type>::type>::type
    >
    : row_buffer_helper<typename View::reference>
{
    row_buffer_helper_view( int width ) 
        : row_buffer_helper<typename View::reference>(width)
    {}
};


template< typename IsBitAligned
        , typename Buffer
        >
struct swap_bits_fn
{
   swap_bits_fn( bool ) {}

   void operator() ( Buffer& ) {}
};

template<>
struct swap_bits_fn< boost::mpl::true_
                   , std::vector< unsigned char > >
{
   swap_bits_fn( bool swap )
   {
      for( int i = 0; i < 256; ++i )
      {
         _lookup[i] = swap_bits( i );
      }

      _swap_bits = swap;
   }

   void operator() ( std::vector< unsigned char >& buffer )
   {
      typedef swap_bits_fn< boost::mpl::true_, std::vector< unsigned char > > tt;

      if( _swap_bits )
      {
         std::transform( buffer.begin()
                       , buffer.end()
                       , buffer.begin()
                       , boost::bind( &tt::_swap, *this, _1 ));
      }
   }
 
 private:
 
   unsigned char _swap( unsigned char byte ) const
   {
      return _lookup[ byte ];
   }
 
 private:
 
   boost::array< unsigned char, 256 > _lookup;
   bool _swap_bits;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_BASE_HPP_INCLUDED
