/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNG_HPP_INCLUDED 

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief
/// \author Christian Henning, Andreas Pokorny \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <png.h>
}

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>
#include <boost/gil/extension/io_new/io.hpp>
#include <boost/gil/extension/io_new/png_tags.hpp>
#include <boost/gil/extension/io_new/detail/png_supported_types.hpp>

namespace boost{ namespace gil {

template<typename PixelType>
struct is_supported<PixelType,png_tag>
    //!\todo this should be lazy: -- channel_type is undefined for non-homogenous pixel types
    : mpl::bool_<detail::png_rw_support<
                    typename channel_type<PixelType>::type,
                    typename color_space_type<PixelType>::type
                    >::is_supported>
{};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNG_HPP_INCLUDED
