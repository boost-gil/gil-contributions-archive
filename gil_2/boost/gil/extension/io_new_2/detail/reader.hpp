/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_READER_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_READER_HPP_INCLUDED 

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning and Andreas Pokorny \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

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
class reader;

}}}

#endif 

