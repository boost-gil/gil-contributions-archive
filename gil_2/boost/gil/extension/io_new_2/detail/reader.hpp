/*
  Copyright 2007-2008 Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_READER_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_READER_HPP_INCLUDED 

namespace boost{ namespace gil{ namespace detail

/**
 * reader of the gil io extension cf to \ref iobackend . 
 *
 * Should be specialized using a valid FormatTag. 
 * IODevice is to be used for reading from the fil, see \ref IODevice.
 * The ConversionPolicy (see \ref ConversionPolicy) is 
 * either boost::gil::read_and_convert<ColorConv> or 
 * boost::gil::detail::read_and_no_convert. In the former 
 * the constructor with the additional color converter 
 * parameter will be used. 
 */
template<typename IODevice, typename FormatTag, typename ConverstionPolicy>
struct reader;

}}}

#endif 

