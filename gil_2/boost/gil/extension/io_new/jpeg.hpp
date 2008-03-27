/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_JPEG_HPP_INCLUDED 

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief
/// \author Christian Henning, Andreas Pokorny \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/detail/jpeg_supported_types.hpp>

namespace boost{ namespace gil {

template<typename PixelType>
struct is_supported<PixelType,jpeg_tag>
    : mpl::bool_<detail::jpeg_rw_support<
                    typename channel_type<PixelType>::type,
                    typename color_space_type<PixelType>::type
                    >::is_supported>
{};

} // namespace boost
} // namespace gil

#endif // BOOST_GIL_EXTENSION_IO_JPEG_HPP_INCLUDED