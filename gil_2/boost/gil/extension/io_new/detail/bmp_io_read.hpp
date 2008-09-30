/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include "base.hpp"
#include "reader_base.hpp"
#include "io_device.hpp"
#include "typedefs.hpp"
#include "jpeg_supported_types.hpp"

namespace boost { namespace gil { namespace detail {

/// Compute the consecutive zero bits on the right
template< typename T >
inline
unsigned trailing_zeros( T x )
throw()
{
    unsigned n = 0;

    x = ~x & (x - 1);

    while( x )
    {
        n = n + 1;
        x = x >> 1;
    }

    return n;
}

/// Counts a bit-set
template< typename T >
inline
unsigned count_ones( T x )
throw()
{
    unsigned n = 0;

    while( x )
    {
	    // clear the least significant bit set
	    x &= x - 1;
	    ++n
    }

    return n;
}


static const int header_size     = 14;
static const int win32_info_size = 40;
static const int os2_info_size   = 12;
static const int bm_signature    = 0x4D42;


struct color_map
{
    unsigned int blue;   // Blue bits mask
    unsigned int green;  // Green bits mask
    unsigned int red;    // Red bits mask
    unsigned int unused; // Reserved
};

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


template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , bmp_tag
            , ConversionPolicy
            > 
    : public reader_base< jpeg_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    reader( Device& device )
    : _io_dev( device )
    {}

    reader( Device&     device
          , const cc_t& cc
          )
    : _io_dev( device )
    , reader_base< bmp_tag
                 , ConversionPolicy
                 >( cc )
    {}

    image_read_info< bmp_tag > get_info()
    {
        // read file header

        // the magic number used to identify the BMP file: 
        // 0x42 0x4D (ASCII code points for B and M)
        if( _io_dev.read_int16() == 0x4D42 )
        {
            io_error( "Wrong magic number for bmp file." );
        }

        // the size of the BMP file in bytes
        uint32_t size = _io_dev.read_int32();

        // reserved; actual value depends on the application that creates the image
        _io_dev.read_int16();
        // reserved; actual value depends on the application that creates the image
        _io_dev.read_int16();
        
        _info._offset = _io_dev.read_int32();


        // bitmap information

        // the size of this header ( 40 bytes )
        _info._header_size = _io_dev.read_int32();

        if( _info._header_size == win32_info_size )
        {
            _info._width  = _io_dev.read_int32();
            _info._height = _io_dev.read_int32();

            // the number of color planes being used. Must be set to 1.
            _io_dev.read_int16();

            _info._bits_per_pixel = _io_dev.read_int16();

            _info._compression = _io_dev.read_int32();

            _info._image_size = _io_dev.read_int32();

            _info._horizontal_resolution = _io_dev.read_int32();
            _info._vertical_resolution   = _io_dev.read_int32();

            _info._num_colors           = _io_dev.read_int32();
            _info._num_important_colors = _io_dev.read_int32();

        }
        else if( _info._header_size == os2_info_size )
        {
            _info._width  = static_cast< bmp_image_width::type  >( _io_dev.read_int16() );
            _info._height = static_cast< bmp_image_height::type >( _io_dev.read_int16() );

            // the number of color planes being used. Must be set to 1.
            _io_dev.read_int16();

            _info._bits_per_pixel = _io_dev.read_int16();

            _info._compression = ct_rgb;

            _info._image_size = _io_dev.read_int32();

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

        return _info;
    }

    template< typename View >
    void apply( const View& view )
    {
        if( !_info.valid )
        {
            get_info();
        }

        // read the color masks
        color_mask mask;
        if( _info_header.what == ct_bitfield )
        {
            mask.red.mask    = _io_dev.read_int32();
            mask.green.mask  = _io_dev.read_int32();
            mask.blue.mask   = _io_dev.read_int32();

            mask.red.width   = count_ones( mask.red.mask   );
            mask.green.width = count_ones( mask.green.mask );
            mask.blue.width  = count_ones( mask.blue.mask  );

            mask.red.shift   = trailing_zeros( mask.red.mask   );
            mask.green.shift = trailing_zeros( mask.green.mask );
            mask.blue.shift  = trailing_zeros( mask.blue.mask  );
        }
        else if( _info_header.what == ct_rgb )
        {
            switch( _info_header.bpp )
            {
                case 15:
                case 16:
                {
                    mask.red.mask   = 0x007C00; mask.red.width   = 5; mask.red.shift   = 10;
                    mask.green.mask = 0x0003E0; mask.green.width = 5; mask.green.shift =  5;
                    mask.blue.mask  = 0x00001F; mask.blue.width  = 5; mask.blue.shift  =  0;
                    break;
                }

                case 24:
                case 32:
                {
                    mask.red.mask   = 0xFF0000; mask.red.width   = 8; mask.red.shift   = 16;
                    mask.green.mask = 0x00FF00; mask.green.width = 8; mask.green.shift =  8;
                    mask.blue.mask  = 0x0000FF; mask.blue.width  = 8; mask.blue.shift  =  0;
                    break;
                }
            }
        }
        else
        {
            io_error( "bmp_reader::apply(): unsupported BMP compression" );
        }

        // Read the color map.
        std::vector<color_map> palette;

        if( _info_header.bits_per_plane <= 8 )
        {
            int entries = _info_header.colors;

            if( entries == 0 )
            {
                entries = 1 << _info.bits_per_plane;
            }

            palette.resize( entries );

            for( int i = 0; i < entries; ++i )
            {
                palette[i].blue  = read_int8();
                palette[i].green = read_int8();
                palette[i].red   = read_int8();

                if( _info.entry > 3 )
                {
                    read_int8();
                }

            } // for
        } // if


        // go to first byte of bitmap data
        _io_dev.seek( _info._offset );


        // the row pitch must be multiple 4 bytes
        int pitch;

        if( _info_header.bpp < 8 )
        {
            pitch = ((_info_header.width * _info_header.bpp) + 7) >> 3;
        }
        else
        {
            pitch = _info_header.width * ((_info_header.bpp + 7) >> 3);
        }

        pitch = (pitch + 3) & ~3;

        // read the raster
        std::vector<byte_t> row(pitch);

        int ybeg = 0;
        int yend = _info_header.height;
        int yinc = 1;

        if( _info._height > 0 )
        {
            // the image is upside down
            ybeg = _info._height - 1;
            yend = -1;
            yinc = -1;
        }

        const color_map* pal = 0;

		if( palette.size() > 0 )
		{
			pal = &palette.front();
		}

        for( int y = ybeg; y != yend; y += yinc )
        {
            typedef typename color_space_type< View >::type Spc;

            read( &row.front(), pitch );
        }
    }

private:

    Device& _io_dev;
    image_read_info< bmp_tag > _info;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED
