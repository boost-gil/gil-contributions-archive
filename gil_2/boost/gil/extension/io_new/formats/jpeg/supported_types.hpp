/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_SUPPORTED_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>

namespace boost { namespace gil { namespace detail {

// Read support

template< typename Channel
        , typename ColorSpace
        >
struct jpeg_read_support : read_support_false {};

template<>
struct jpeg_read_support< bits8
                        , rgb_t
                        > : read_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_RGB );
};

template<>
struct jpeg_read_support< bits8
                        , cmyk_t
                        > : read_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_CMYK );
};

template<>
struct jpeg_read_support< bits8
                        , gray_t
                        > : read_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_GRAYSCALE );
};

// Write support

template< typename Channel
        , typename ColorSpace
        >
struct jpeg_write_support : write_support_false
{};

template<>
struct jpeg_write_support< bits8
                         , gray_t
                         > : write_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_GRAYSCALE );
};

template<>
struct jpeg_write_support< bits8
                         , rgb_t
                         > : write_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_RGB );
};

template<>
struct jpeg_write_support< bits8
                         , cmyk_t
                         > : write_support_true
{
    BOOST_STATIC_CONSTANT( J_COLOR_SPACE, color_type = JCS_CMYK );
};

} // namespace detail

template< typename Pixel >
struct is_read_supported< Pixel
                        , jpeg_tag
                        > 
    : mpl::bool_< detail::jpeg_read_support< typename channel_type< Pixel >::type
                                           , typename color_space_type< Pixel >::type
                                           >::is_supported
                >
{};

template< typename Pixel >
struct is_write_supported< Pixel
                         , jpeg_tag
                         >
    : mpl::bool_< detail::jpeg_write_support< typename channel_type< Pixel >::type
                                            , typename color_space_type< Pixel >::type
                                            >::is_supported
                > 
{};


} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_JPEG_SUPPORTED_TYPES_HPP
