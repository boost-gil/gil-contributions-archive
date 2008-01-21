/*
  Copyright 2007-2008 Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_TYPES_HPP

//! \author Andreas Pokorny

namespace boost { namespace gil { namespace detail {

template <typename Channel,typename ColorSpace>
struct tiff_rw_support {
    BOOST_STATIC_CONSTANT(bool,is_supported=false);
    BOOST_STATIC_CONSTANT(int,bit_depth=0);
};

template <>
struct tiff_rw_support<bits8,gray_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=8);
};

template <>
struct tiff_rw_support<bits8,rgb_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=8);
};

template <>
struct tiff_rw_support<bits8,rgba_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=8);
};

template <>
struct tiff_rw_support<bits16,gray_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=16);
};

template <>
struct tiff_rw_support<bits16,rgb_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=16);
};

template <>
struct tiff_rw_support<bits16,rgba_t> {
    BOOST_STATIC_CONSTANT(bool,is_supported=true);
    BOOST_STATIC_CONSTANT(int,bit_depth=16);
};

} // namespace detail
} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_TYPES_HPP
