/*
  Copyright 2007-2008 Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNG_HPP_INCLUDED 

#include <png.h>
#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/png_tags.hpp>
#include <boost/gil/extension/io_new_2/detail/png_supported_types.hpp>
#include <boost/gil/extension/io_new_2/detail/png_io.hpp>

namespace boost{ namespace gil {

template<typename PixelType>
struct is_supported<PixelType,png_tag>
    //!\todo this should be lazy: -- channel_type is undefined for non-homogenous pixel types
    : mpl::bool_<detail::png_rw_support<
                    typename channel_type<PixelType>::type,
                    typename color_space_type<PixelType>::type
                    >::is_supported>
{};
}}
#endif

