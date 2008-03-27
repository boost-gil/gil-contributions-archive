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


namespace boost { namespace gil { namespace detail {

template<typename ChannelT,typename ColorSpace>
struct jpeg_rw_support
{
    BOOST_STATIC_CONSTANT(bool,is_supported=false);
};

template<>
struct jpeg_rw_support<bits8, rgb_t>
{
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(J_COLOR_SPACE,color_type=JCS_RGB);
};

template<>
struct jpeg_rw_support<bits8, cmyk_t>
{
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(J_COLOR_SPACE,color_type=JCS_CMYK);
};

template<>
struct jpeg_rw_support<bits8, gray_t>
{
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(J_COLOR_SPACE,color_type=JCS_GRAYSCALE);
};

} // namespace detail
} // namespace gil
} // namespace boost


#endif


