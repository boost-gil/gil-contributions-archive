/*
  Copyright 2007-2008 Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_JPEG_HPP_INCLUDED 

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/detail/jpeg_io.hpp>
#include <boost/gil/extension/io_new_2/detail/jpeg_supported_types.hpp>

namespace boost{ namespace gil {

template<typename PixelType>
struct is_supported<PixelType,jpeg_tag>
    : mpl::bool_<detail::jpeg_rw_support<
                    typename channel_type<PixelType>::type,
                    typename color_space_type<PixelType>::type
                    >::is_supported>
{};

}}

#endif

