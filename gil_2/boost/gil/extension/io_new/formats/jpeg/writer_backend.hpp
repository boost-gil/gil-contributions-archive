/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITER_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITER_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include "base.hpp"

namespace boost { namespace gil {

///
/// JPEG Writer Backend
///
template< typename Device >
struct writer_backend< Device
                     , jpeg_tag
                     >
    : public jpeg_io_base
{
    writer_backend( Device&                             io_dev
                  , const image_write_info< jpeg_tag >& info
                  )
    : _io_dev( io_dev )
    , _info( info )
    {
        _cinfo.err         = jpeg_std_error( &_jerr );
        _cinfo.client_data = this;

        // Error exit handler: does not return to caller.
        _jerr.error_exit = &writer< Device, jpeg_tag >::error_exit;

        // Fire exception in case of error.
        if( setjmp( _mark )) { raise_error(); }

        _dest._jdest.free_in_buffer      = sizeof( buffer );
        _dest._jdest.next_output_byte    = buffer;
        _dest._jdest.init_destination    = reinterpret_cast< void(*)   ( j_compress_ptr ) >( &writer< Device, jpeg_tag >::init_device  );
        _dest._jdest.empty_output_buffer = reinterpret_cast< boolean(*)( j_compress_ptr ) >( &writer< Device, jpeg_tag >::empty_buffer );
        _dest._jdest.term_destination    = reinterpret_cast< void(*)   ( j_compress_ptr ) >( &writer< Device, jpeg_tag >::close_device );
        _dest._this = this;

        jpeg_create_compress( &_cinfo  );
        _cinfo.dest = &_dest._jdest;
    }

    ~writer_backend()
    {
        jpeg_finish_compress ( &_cinfo );
        jpeg_destroy_compress( &_cinfo );
    }

protected:

    struct gil_jpeg_destination_mgr
    {
        jpeg_destination_mgr _jdest;
        writer_backend< Device
                      , jpeg_tag
                      >* _this;
    };

    static void init_device( jpeg_compress_struct* cinfo )
    {
        gil_jpeg_destination_mgr* dest = reinterpret_cast< gil_jpeg_destination_mgr* >( cinfo->dest );

        dest->_jdest.free_in_buffer   = sizeof( dest->_this->buffer );
        dest->_jdest.next_output_byte = dest->_this->buffer;
    }

    static boolean empty_buffer( jpeg_compress_struct* cinfo )
    {
        gil_jpeg_destination_mgr* dest = reinterpret_cast< gil_jpeg_destination_mgr* >( cinfo->dest );

        dest->_this->_io_dev.write( dest->_this->buffer
                                  , buffer_size
                                  );

        writer<Device,jpeg_tag>::init_device( cinfo );
        return 1;
    }

    static void close_device( jpeg_compress_struct* cinfo )
    {
        writer_backend< Device
                      , jpeg_tag
                      >::empty_buffer( cinfo );

        gil_jpeg_destination_mgr* dest = reinterpret_cast< gil_jpeg_destination_mgr* >( cinfo->dest );

        dest->_this->_io_dev.flush();
    }

    void raise_error()
    {
        io_error( "Cannot write jpeg file." );
    }

    static void error_exit( j_common_ptr cinfo )
    {
        writer< Device, jpeg_tag >* mgr = reinterpret_cast< writer< Device, jpeg_tag >* >( cinfo->client_data );

        longjmp( mgr->_mark, 1 );
    }

public:

    Device& _io_dev;

    image_write_info< jpeg_tag > _info;

    jpeg_compress_struct _cinfo;

    gil_jpeg_destination_mgr _dest;

    static const unsigned int buffer_size = 1024;
    JOCTET buffer[buffer_size];
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITER_BACKEND_HPP
