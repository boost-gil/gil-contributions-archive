/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_RAW_IO_WRITER_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_RAW_IO_WRITER_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/raw_tags.hpp>

namespace boost { namespace gil {

///
/// RAW Writer Backend
///
template< typename Device >
struct writer_backend< Device
                     , raw_tag
                     >
{
    writer_backend( const Device&                      io_dev
                  , const image_write_info< raw_tag >& info
                  )
    : _io_dev( io_dev )
    , _info  ( info   )
    {}

public:

    Device _io_dev;

    image_write_info< raw_tag > _info;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_RAW_IO_WRITER_BACKEND_HPP
