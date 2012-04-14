/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_BASE_HPP
#define BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_BASE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/png_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

namespace boost { namespace gil { 

template< typename Device >
class png_io_base
{
public:
    png_io_base( const Device& io_dev )
    : _io_dev( io_dev )
    {}

    ~png_io_base()
    {
        _io_dev.set_close( true );
    }

protected:

    static void read_data( png_structp png_ptr
                         , png_bytep   data
                         , png_size_t length
                         )
    {
        static_cast<Device*>(png_get_io_ptr(png_ptr) )->read( data
                                                            , length );
    }

    static void write_data( png_structp png_ptr
                          , png_bytep   data
                          , png_size_t  length
                          )
    {
        static_cast<Device*>( png_get_io_ptr( png_ptr ))->write( data
                                                               , length );
    }

    static void flush( png_structp png_ptr )
    {
        static_cast<Device*>(png_get_io_ptr(png_ptr) )->flush();
    }


    static int read_user_chunk_callback( png_struct*        /* png_ptr */
                                       , png_unknown_chunkp /* chunk */
                                       )
    {
        // @todo
        return 0;
    }

    static void read_row_callback( png_structp /* png_ptr    */
                                 , png_uint_32 /* row_number */
                                 , int         /* pass       */
                                 )
    {
        // @todo
    }

protected:

    Device _io_dev;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_BASE_HPP
