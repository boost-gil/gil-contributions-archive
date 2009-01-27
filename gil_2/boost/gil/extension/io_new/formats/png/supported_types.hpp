/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_PNG_SUPPORTED_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_GRAY_ALPHA
#include <boost/gil/extension/toolbox/gray_alpha.hpp>
#endif // ENABLE_GRAY_ALPHA

namespace boost { namespace gil { namespace detail {

static const size_t PNG_BYTES_TO_CHECK = 4;

// Read support

template< typename Channel
        , typename ColorSpace
        >
struct png_read_support : read_support_false
{};

template< int BitDepth
        , int ColorType
        >
struct png_rw_support_base
{
    BOOST_STATIC_CONSTANT( int, bit_depth  = BitDepth  );
    BOOST_STATIC_CONSTANT( int, color_type = ColorType );
};

template< typename BitField
        , bool     Mutable
        >
struct png_read_support< packed_dynamic_channel_reference< BitField
                                                         , 1
                                                         , Mutable
                                                         >
                       , gray_t
                       > : read_support_true
                         , png_rw_support_base< 1
                                              , PNG_COLOR_TYPE_GRAY
                                              > {};

template< typename BitField
        , bool     Mutable
        >
struct png_read_support< packed_dynamic_channel_reference< BitField
                                                         , 2
                                                         , Mutable
                                                         >
                       , gray_t
                       > : read_support_true
                         , png_rw_support_base< 2
                                              , PNG_COLOR_TYPE_GRAY
                                              > {};

template< typename BitField
        , bool     Mutable
        >
struct png_read_support< packed_dynamic_channel_reference< BitField
                                                         , 4
                                                         , Mutable
                                                         >
                       , gray_t
                       > : read_support_true
                         , png_rw_support_base< 4
                                              , PNG_COLOR_TYPE_GRAY
                                              > {};

template<>
struct png_read_support< bits8
                       , gray_t
                       > : read_support_true
                         , png_rw_support_base< 8
                                              , PNG_COLOR_TYPE_GRAY
                                              > {};

#ifdef ENABLE_GRAY_ALPHA
template<>
struct png_read_support< bits8
                       , gray_alpha_t
                       > : read_support_true
                         , png_rw_support_base< 8
                                              , PNG_COLOR_TYPE_GA
                                              > {};
#endif // ENABLE_GRAY_ALPHA

template<>
struct png_read_support< bits8
                       , rgb_t
                       > : read_support_true
                         , png_rw_support_base< 8
                                              , PNG_COLOR_TYPE_RGB
                                              > {};

template<>
struct png_read_support< bits8
                       , rgba_t
                       > : read_support_true
                         , png_rw_support_base< 8
                                              , PNG_COLOR_TYPE_RGBA
                                              > {};

template<>
struct png_read_support< bits16
                       , gray_t
                       > : read_support_true
                         , png_rw_support_base< 16
                                              , PNG_COLOR_TYPE_GRAY
                                              > {};

template<>
struct png_read_support< bits16
                       , rgb_t
                       > : read_support_true
                         , png_rw_support_base< 16
                                              , PNG_COLOR_TYPE_RGB
                                              > {};

template<>
struct png_read_support< bits16
                       , rgba_t
                       > : read_support_true
                         , png_rw_support_base< 16
                                              , PNG_COLOR_TYPE_RGBA
                                              > {};

#ifdef ENABLE_GRAY_ALPHA
template<>
struct png_read_support< bits16
                       , gray_alpha_t
                       > : read_support_true
                         , png_rw_support_base< 16
                                              , PNG_COLOR_TYPE_GA
                                              > {};
#endif // ENABLE_GRAY_ALPHA

// Write support

template< typename Channel
        , typename ColorSpace
        >
struct png_write_support : write_support_false
{};

template< typename BitField
        , bool     Mutable
        >
struct png_write_support< packed_dynamic_channel_reference< BitField
                                                          , 1
                                                          , Mutable
                                                          >
                        , gray_t
                        > : write_support_true
                          , png_rw_support_base< 1
                                               , PNG_COLOR_TYPE_GRAY
                                               >
{};

template< typename BitField
        , bool     Mutable
        >
struct png_write_support< packed_dynamic_channel_reference< BitField
                                                          , 2
                                                          , Mutable
                                                          >
                        , gray_t
                        > : write_support_true
                          , png_rw_support_base< 2
                                               , PNG_COLOR_TYPE_GRAY
                                               >
{};

template< typename BitField
        , bool     Mutable
        >
struct png_write_support< packed_dynamic_channel_reference< BitField
                                                          , 4
                                                          , Mutable
                                                          >
                        , gray_t
                        > : write_support_true
                          , png_rw_support_base< 4
                                               , PNG_COLOR_TYPE_GRAY
                                               >
{};

template<>
struct png_write_support< bits8
                        , gray_t
                        > : write_support_true
                          , png_rw_support_base< 8
                                               , PNG_COLOR_TYPE_GRAY
                                               >
{};

#ifdef ENABLE_GRAY_ALPHA
template<>
struct png_write_support< bits8
                        , gray_alpha_t
                        > : write_support_true
                          , png_rw_support_base< 8
                                               , PNG_COLOR_TYPE_GA
                                               >
{};
#endif // ENABLE_GRAY_ALPHA

template<>
struct png_write_support< bits8
                        , rgb_t
                        > : write_support_true
                          , png_rw_support_base< 8
                                               , PNG_COLOR_TYPE_RGB
                                               >
{};

template<>
struct png_write_support< bits8
                        , rgba_t
                        > : write_support_true
                          , png_rw_support_base< 8
                                               , PNG_COLOR_TYPE_RGBA
                                               >
{};

template<>
struct png_write_support< bits16
                        , gray_t
                        > : write_support_true
                          , png_rw_support_base< 16
                                               , PNG_COLOR_TYPE_GRAY
                                               >
{};

template<>
struct png_write_support< bits16
                        , rgb_t
                        > : write_support_true
                          , png_rw_support_base< 16
                                               , PNG_COLOR_TYPE_RGB
                                               >
{};

template<>
struct png_write_support< bits16
                        , rgba_t
                        > : write_support_true
                          , png_rw_support_base< 16
                                               , PNG_COLOR_TYPE_RGBA
                                               >
{};

#ifdef ENABLE_GRAY_ALPHA
template<>
struct png_write_support< bits16
                        , gray_alpha_t
                        > : write_support_true
                          , png_rw_support_base< 16
                                               , PNG_COLOR_TYPE_GA
                                               >
{};
#endif // ENABLE_GRAY_ALPHA


} // namespace detail

template< typename Pixel >
struct is_read_supported< Pixel
                        , png_tag
                        >
    : mpl::bool_< detail::png_read_support< typename channel_type< Pixel >::type
                                          , typename color_space_type< Pixel >::type
                                          >::is_supported
                >
{};

template< typename Pixel >
struct is_write_supported< Pixel
                         , png_tag
                         >
    : mpl::bool_< detail::png_write_support< typename channel_type< Pixel >::type
                                           , typename color_space_type< Pixel >::type
                                           >::is_supported
                > 
{};

} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_PNG_SUPPORTED_TYPES_HPP
