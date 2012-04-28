/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_WRITER_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_BMP_IO_WRITER_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/bmp_tags.hpp>

namespace boost { namespace gil {

///
/// BMP Writer Backend
///
template< typename Device >
struct writer_backend< Device
                     , bmp_tag
                     >
{
    writer_backend( Device& device
                  , const image_write_info< bmp_tag >& info
                  )
    : _out( device )
    , _info( info )
    {}

    Device& _out;

    image_write_info< bmp_tag > _info;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_WRITER_BACKEND_HPP
