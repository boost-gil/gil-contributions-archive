/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <csetjmp>
#include <vector>
#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "base.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil {

template<typename Device>
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

    Device &_io_dev;

    image_read_settings< jpeg_tag > _settings;
    image_read_info< jpeg_tag >     _info;


    struct gil_jpeg_source_mgr
    {
        jpeg_source_mgr _jsrc;
        reader_backend* _this;
    };

    gil_jpeg_source_mgr _src;

    // libjpeg default is 4096 - see jdatasrc.c
    JOCTET buffer[4096];
};

template< typename Device >
class scanline_reader< Device
                     , jpeg_tag
                     >
    : public reader_backend< Device
                           , jpeg_tag
                           >
{
public:

    typedef reader_backend< Device, jpeg_tag > backend_t;

public:
    scanline_reader( Device&                                device
                   , const image_read_settings< jpeg_tag >& settings
                   )
    : reader_backend< Device
                    , jpeg_tag
                     >( device
                      , settings
                      )
    {}

    void read_header()
    {
        _info._width          = this->_cinfo.image_width;
        _info._height         = this->_cinfo.image_height;
        _info._num_components = this->_cinfo.num_components;
        _info._color_space    = this->_cinfo.jpeg_color_space;
        _info._data_precision = this->_cinfo.data_precision;

        _info._density_unit = this->_cinfo.density_unit;
        _info._x_density    = this->_cinfo.X_density;
        _info._y_density    = this->_cinfo.Y_density;

        // obtain real world dimensions
        // taken from https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/read.cpp#cl-62
        jpeg_calc_output_dimensions( &this->_cinfo );

        double units_conversion = 0;
        if (this->_cinfo.density_unit == 1) // dots per inch
        {
            units_conversion = 25.4; // millimeters in an inch
        }
        else if (this->_cinfo.density_unit == 2) // dots per cm
        {
            units_conversion = 10; // millimeters in a centimeter
        }

        _info._pixel_width_mm  = this->_cinfo.X_density ? (this->_cinfo.output_width  / double(this->_cinfo.X_density)) * units_conversion : 0;
        _info._pixel_height_mm = this->_cinfo.Y_density ? (this->_cinfo.output_height / double(this->_cinfo.Y_density)) * units_conversion : 0;
    }

    void initialize()
    {
        jpeg_decompress_struct& cinfo = this->_cinfo;
        cinfo.dct_method = this->_settings._dct_method;

        io_error_if( jpeg_start_decompress( &this->_cinfo ) == false
                    , "Cannot start decompression." );

        //switch( this->_info._color_space )
        //{
        //    case JCS_GRAYSCALE: { read_rows< gray8_pixel_t >( view ); break; }
        //    case JCS_RGB:       { read_rows< rgb8_pixel_t  >( view ); break; }

        //    //!\todo add Y'CbCr? We loose image quality when reading JCS_YCbCr as JCS_RGB
        //    case JCS_YCbCr:     { read_rows< rgb8_pixel_t  >( view ); break; }

        //    case JCS_CMYK:      { read_rows< cmyk8_pixel_t >( view ); break; }

        //    //!\todo add Y'CbCrK? We loose image quality when reading JCS_YCCK as JCS_CMYK
        //    case JCS_YCCK:
        //    {
        //        this->_cinfo.out_color_space = JCS_CMYK;
        //        read_rows< cmyk8_pixel_t >( view );

        //        break;
        //    }
        //    default: { io_error( "Unsupported jpeg color space." ); }
        //}
    }

    void read( byte_t* dst, int pos )
    {
        // Fire exception in case of error.
        if( setjmp( this->_mark )) { this->raise_error(); }

        // read data
        read_scanline( dst );
    }

    void clean_up()
    {
        ///@todo
        //jpeg_finish_decompress ( &this->_cinfo );
    }

private:

    void read_scanline( byte_t* dst )
    {
        JSAMPLE *row_adr = reinterpret_cast< JSAMPLE* >( dst );

        // Read data.
        io_error_if( jpeg_read_scanlines( &this->_cinfo
                                        , &row_adr
                                        , 1
                                        ) != 1
                    , "jpeg_read_scanlines: fail to read JPEG file"
                    );

    }
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP
