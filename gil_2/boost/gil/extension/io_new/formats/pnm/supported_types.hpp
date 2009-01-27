/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_PNM_SUPPORTED_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>

namespace boost { namespace gil { namespace detail {

// Read Support

template< typename Channel
        , typename ColorSpace
        >
struct pnm_read_support : read_support_false {};

template< typename BitField, bool Mutable >
struct pnm_read_support< packed_dynamic_channel_reference< BitField
                                                         , 1
                                                         , Mutable
                                                         >
                       , gray_t
                       > : read_support_true {};

template<>
struct pnm_read_support< bits8
                       , gray_t
                       > : read_support_true {};


template<>
struct pnm_read_support< bits8
                       , rgb_t
                       > : read_support_true {};

// Write support

template< typename Channel
        , typename ColorSpace
        >
struct pnm_write_support : write_support_false
{};

template< typename BitField, bool Mutable >
struct pnm_write_support< packed_dynamic_channel_reference< BitField
                                                          , 1
                                                          , Mutable
                                                          >
                        , gray_t
                        > : write_support_true {};

template<>
struct pnm_write_support< bits8
                        , gray_t
                        > : write_support_true {};


template<>
struct pnm_write_support< bits8
                        , rgb_t
                        > : write_support_true {};

} // namespace detail

template< typename Pixel >
struct is_read_supported< Pixel
                        , pnm_tag
                        >
    : mpl::bool_< detail::pnm_read_support< typename channel_type< Pixel >::type
                                          , typename color_space_type< Pixel >::type
                                          >::is_supported
                > {};

template< typename Pixel >
struct is_write_supported< Pixel
                         , pnm_tag
                         >
    : mpl::bool_< detail::pnm_write_support< typename channel_type< Pixel >::type
                                           , typename color_space_type< Pixel >::type
                                           >::is_supported
                > {};

} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_PNM_SUPPORTED_TYPES_HPP
