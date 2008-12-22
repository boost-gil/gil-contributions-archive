/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_BMP_SUPPORTED_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date   2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>

namespace boost { namespace gil { namespace detail {

template< typename Channel
        , typename ColorSpace
        >
struct pnm_rw_support
{
    BOOST_STATIC_CONSTANT(bool,is_supported=false);
};

struct pnm_rw_support_true
{
    BOOST_STATIC_CONSTANT( bool, is_supported = true );
};

template< typename BitField, bool Mutable >
struct pnm_rw_support< packed_dynamic_channel_reference< BitField
                                                       , 1
                                                       , Mutable
                                                       >
                     , gray_t
                     > : pnm_rw_support_true {};

template< typename BitField, bool Mutable >
struct pnm_rw_support< packed_dynamic_channel_reference< BitField
                                                       , 4
                                                       , Mutable
                                                       >
                     , gray_t
                     > : pnm_rw_support_true {};

template<>
struct pnm_rw_support< bits8
                     , gray_t
                     > : pnm_rw_support_true {};


template<>
struct pnm_rw_support< bits8
                     , rgb_t
                     > : pnm_rw_support_true {};

template<>
struct pnm_rw_support< bits8
                     , rgba_t
                     > : pnm_rw_support_true {};

} // namespace detail

template< typename Pixel >
struct is_supported< Pixel
                   , bmp_tag
                   >
: mpl::bool_< detail::pnm_rw_support<
                     typename channel_type< Pixel >::type,
                     typename color_space_type< Pixel >::type
                     >::is_supported 
            >
{};

} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_BMP_SUPPORTED_TYPES_HPP
