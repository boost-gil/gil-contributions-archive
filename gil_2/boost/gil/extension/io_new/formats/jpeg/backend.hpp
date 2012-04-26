/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/jpeg_tags.hpp>

namespace boost { namespace gil {

///
/// JPEG Backend
///
template< typename Device >
struct reader_backend< Device
                     , jpeg_tag
                     >
    : public detail::jpeg_io_base
{

    //
    // Constructor
    //
    reader_backend( Device&                               file
                  , const image_read_settings< jpeg_tag >& settings
                  )
    : _io_dev( file )
    , _settings( settings )
    , _info()

    , _scanline_length( 0 )
    {
        _cinfo.err         = jpeg_std_error( &_jerr );
        _cinfo.client_data = this;

        // Error exit handler: does not return to caller.
        _jerr.error_exit = &reader_backend::error_exit;

        if( setjmp( _mark )) { raise_error(); }

        _src._jsrc.bytes_in_buffer   = 0;
        _src._jsrc.next_input_byte   = buffer;
        _src._jsrc.init_source       = reinterpret_cast< void(*)   ( j_decompress_ptr )>( &reader_backend< Device, jpeg_tag >::init_device );
        _src._jsrc.fill_input_buffer = reinterpret_cast< boolean(*)( j_decompress_ptr )>( &reader_backend< Device, jpeg_tag >::fill_buffer );
        _src._jsrc.skip_input_data   = reinterpret_cast< void(*)   ( j_decompress_ptr
                                                                   , long num_bytes
                                                                   ) >( &reader_backend< Device, jpeg_tag >::skip_input_data );
        _src._jsrc.term_source       = reinterpret_cast< void(*)   ( j_decompress_ptr ) >( &reader_backend< Device, jpeg_tag >::close_device );
        _src._jsrc.resync_to_restart = jpeg_resync_to_restart;
        _src._this = this;

        jpeg_create_decompress( &_cinfo );

        _cinfo.src = &_src._jsrc;

        jpeg_read_header( &_cinfo
                        , TRUE
                        );

        io_error_if( _cinfo.data_precision != 8
                   , "Image file is not supported."
                   );
    }

    //
    // Destructor
    //
    ~reader_backend()
    {
        jpeg_destroy_decompress( &_cinfo );
    }

    /// Read image header.
    void read_header()
    {
        _info._width          = _cinfo.image_width;
        _info._height         = _cinfo.image_height;
        _info._num_components = _cinfo.num_components;
        _info._color_space    = _cinfo.jpeg_color_space;
        _info._data_precision = _cinfo.data_precision;

        _info._density_unit = _cinfo.density_unit;
        _info._x_density    = _cinfo.X_density;
        _info._y_density    = _cinfo.Y_density;

        // obtain real world dimensions
        // taken from https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/read.cpp#cl-62
        jpeg_calc_output_dimensions( &this->_cinfo );

        double units_conversion = 0;
        if (_cinfo.density_unit == 1) // dots per inch
        {
            units_conversion = 25.4; // millimeters in an inch
        }
        else if (_cinfo.density_unit == 2) // dots per cm
        {
            units_conversion = 10; // millimeters in a centimeter
        }

        _info._pixel_width_mm  = _cinfo.X_density ? (_cinfo.output_width  / double(_cinfo.X_density)) * units_conversion : 0;
        _info._pixel_height_mm = _cinfo.Y_density ? (_cinfo.output_height / double(_cinfo.Y_density)) * units_conversion : 0;
    }

    /// Return image read settings.
    const image_read_settings< bmp_tag >& get_settings()
    {
        return _settings();
    }
    
    /// Return image header info.
    const image_read_info< bmp_tag >& get_info()
    {
        return _info();
    }

protected:

    // Taken from jerror.c
    /*
     * Error exit handler: must not return to caller.
     *
     * Applications may override this if they want to get control back after
     * an error.  Typically one would longjmp somewhere instead of exiting.
     * The setjmp buffer can be made a private field within an expanded error
     * handler object.  Note that the info needed to generate an error message
     * is stored in the error object, so you can generate the message now or
     * later, at your convenience.
     * You should make sure that the JPEG object is cleaned up (with jpeg_abort
     * or jpeg_destroy) at some point.
     */
    static void error_exit( j_common_ptr cinfo )
    {
        reader_backend< Device, jpeg_tag >* mgr = reinterpret_cast< reader_backend< Device, jpeg_tag >* >( cinfo->client_data );

        longjmp( mgr->_mark, 1 );
    }

    void raise_error()
    {
        // we clean up in the destructor

        io_error( "jpeg is invalid." );
    }

private:

    // See jdatasrc.c for default implementation for the following static member functions.

    static void init_device( jpeg_decompress_struct * cinfo )
    {
        gil_jpeg_source_mgr* src = reinterpret_cast< gil_jpeg_source_mgr* >( cinfo->src );
        src->_jsrc.bytes_in_buffer = 0;
        src->_jsrc.next_input_byte = src->_this->buffer;
    }

    static boolean fill_buffer( jpeg_decompress_struct * cinfo )
    {
        gil_jpeg_source_mgr* src = reinterpret_cast< gil_jpeg_source_mgr* >( cinfo->src );
        size_t count = src->_this->_io_dev.read(src->_this->buffer, sizeof(src->_this->buffer) );

        if( count <= 0 )
        {
            // libjpeg does that: adding an EOF marker
            src->_this->buffer[0] = (JOCTET) 0xFF;
            src->_this->buffer[1] = (JOCTET) JPEG_EOI;
            count = 2;
        }

        src->_jsrc.next_input_byte = src->_this->buffer;
        src->_jsrc.bytes_in_buffer = count;

        return TRUE;
    }

    static void skip_input_data( jpeg_decompress_struct * cinfo, long num_bytes  )
    {
        gil_jpeg_source_mgr* src = reinterpret_cast< gil_jpeg_source_mgr* >( cinfo->src );

        if( num_bytes > 0 )
        {
            while( num_bytes > long( src->_jsrc.bytes_in_buffer ))
            {
                num_bytes -= (long) src->_jsrc.bytes_in_buffer;
                fill_buffer( cinfo );
            }

            src->_jsrc.next_input_byte += num_bytes;
            src->_jsrc.bytes_in_buffer -= num_bytes;
        }
    }

    static void close_device( jpeg_decompress_struct* ) {}

public:

    jpeg_decompress_struct _cinfo;

    Device& _io_dev;

    image_read_settings< jpeg_tag > _settings;
    image_read_info< jpeg_tag >     _info;

    std::size_t _scanline_length;

    struct gil_jpeg_source_mgr
    {
        jpeg_source_mgr _jsrc;
        reader_backend* _this;
    };

    gil_jpeg_source_mgr _src;

    // libjpeg default is 4096 - see jdatasrc.c
    JOCTET buffer[4096];
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_BACKEND_HPP
