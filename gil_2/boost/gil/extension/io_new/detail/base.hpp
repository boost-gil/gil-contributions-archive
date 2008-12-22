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

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_BASE_HPP_INCLUDED
