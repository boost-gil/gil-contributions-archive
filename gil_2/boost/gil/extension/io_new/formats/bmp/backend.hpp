/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_BMP_IO_BACKEND_HPP

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

/// Color channel mask
struct bit_field
{
    unsigned int mask;  // Bit mask at corresponding position
    unsigned int width; // Bit width of the mask
    unsigned int shift; // Bit position from right to left
};

/// BMP color masks
struct color_mask
{
    bit_field red;   // Red bits
    bit_field green; // Green bits
    bit_field blue;  // Blue bits
};


///
/// BMP Backend
///
template< typename Device >
struct reader_backend< Device
                     , bmp_tag
                     >
{
    reader_backend( Device&                               device
                  , const image_read_settings< bmp_tag >& settings
                  )
    : _io_dev  ( device   )
    , _settings( settings )
    , _info()
    , _scanline_length( 0 )
    , _palette()
    {}

    ~reader_backend()
    {
        _io_dev.set_close( true );
    }

    void read_header()
    {
        // the magic number used to identify the BMP file:
        // 0x42 0x4D (ASCII code points for B and M)
        if( _io_dev.read_uint16() == 0x424D )
        {
            io_error( "Wrong magic number for bmp file." );
        }

        // the size of the BMP file in bytes
        _io_dev.read_uint32();

        // reserved; actual value depends on the application that creates the image
        _io_dev.read_uint16();
        // reserved; actual value depends on the application that creates the image
        _io_dev.read_uint16();

        _info._offset = _io_dev.read_uint32();


        // bitmap information

        // the size of this header ( 40 bytes )
        _info._header_size = _io_dev.read_uint32();

        if( _info._header_size == bmp_header_size::_win32_info_size )
        {
            _info._width  = _io_dev.read_uint32();
            _info._height = _io_dev.read_uint32();

            // the number of color planes being used. Must be set to 1.
            _io_dev.read_uint16();

            _info._bits_per_pixel = _io_dev.read_uint16();

            _info._compression = _io_dev.read_uint32();

            _info._image_size = _io_dev.read_uint32();

            _info._horizontal_resolution = _io_dev.read_uint32();
            _info._vertical_resolution   = _io_dev.read_uint32();

            _info._num_colors           = _io_dev.read_uint32();
            _info._num_important_colors = _io_dev.read_uint32();

        }
        else if( _info._header_size == bmp_header_size::_os2_info_size )
        {
            _info._width  = static_cast< bmp_image_width::type  >( _io_dev.read_uint16() );
            _info._height = static_cast< bmp_image_height::type >( _io_dev.read_uint16() );

            // the number of color planes being used. Must be set to 1.
            _io_dev.read_uint16();

            _info._bits_per_pixel = _io_dev.read_uint16();

            _info._compression = bmp_compression::_rgb;

            // not used
            _info._image_size            = 0;
            _info._horizontal_resolution = 0;
            _info._vertical_resolution   = 0;
            _info._num_colors            = 0;
            _info._num_important_colors  = 0;
        }
        else
        {
            io_error( "Invalid BMP info header." );
        }

        _info._valid = true;
    }

public:

    Device _io_dev;

    image_read_settings< bmp_tag > _settings;
    image_read_info< bmp_tag >     _info;

    std::size_t _scanline_length;

    std::vector< rgba8_pixel_t > _palette;

    color_mask _mask;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_BACKEND_HPP
