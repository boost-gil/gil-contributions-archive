/*
    Copyright 2007-2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BASE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_BASE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/type_traits/is_base_of.hpp>
#include <boost/gil/utilities.hpp>

#include "detail/base.hpp"

namespace boost { namespace gil { namespace detail {

///@todo We should use boost::preprocessor here.

typedef unsigned char byte;

typedef bit_aligned_pixel_reference<byte,mpl::vector_c<int,1>,gray_layout_t,true> gray_1b;
typedef bit_aligned_pixel_reference<byte,mpl::vector_c<int,2>,gray_layout_t,true> gray_2b;
typedef bit_aligned_pixel_reference<byte,mpl::vector_c<int,4>,gray_layout_t,true> gray_4b;

typedef bit_aligned_image1_type< 1, gray_layout_t >::type gray1_image_t;
typedef bit_aligned_image1_type< 2, gray_layout_t >::type gray2_image_t;
typedef bit_aligned_image1_type< 4, gray_layout_t >::type gray4_image_t;

} // namespace details
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BASE_HPP_INCLUDED
