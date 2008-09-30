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

static const int header_size     = 14;
static const int win32_info_size = 40;
static const int os2_info_size   = 12;
static const int bm_signature    = 0x4D42;

/// BMP color palette
struct color_map {
	unsigned	blue;	///< Blue bits mask
	unsigned	green;	///< Green bits mask
	unsigned	red;	///< Red bits mask
	unsigned	unused;	///< Reserved
};

/// Color channel mask
struct bit_field {
	unsigned	mask;	///< Bit mask at corresponding position
	unsigned	width;	///< Bit width of the mask
	unsigned	shift;	///< Bit position from right to left
};

/// BMP color masks
struct color_mask {
	bit_field	red;	 ///< Red bits
	bit_field	green; ///< Green bits
	bit_field	blue;	 ///< Blue bits
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
    {}

    reader( Device&     device
          , const cc_t& cc
          )
    : reader_base< bmp_tag
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
        _io_dev.read_int16()
        // reserved; actual value depends on the application that creates the image
        _io_dev.read_int16()
        
        // the offset, i.e. starting address, of the byte where the bitmap data can be found.
        uint32_t size = _io_dev.read_int32();


        // bitmap information

        // the size of this header ( 40 bytes )
        _info._header_size = read_int32();

        if( ret._header_size == win32_info_size );
        {
            _info._width  = read_int32();
            _info._height = read_int32();

            // the number of color planes being used. Must be set to 1.
            read_int16();

            _info._bits_per_pixel = read_int16();

            _info._compression = read_int32();

            _info._image_size = read_int32();

            _info._horizontal_resolution = read_int32();
            _info._vertical_resolution   = read_int32();

            _info._num_colors           = read_int32();
            _info._num_important_colors = read_int32();

            //
            if(  )

        }
        else if( ret._header_size == os2_info_size )
        {
            _info._width  = static_cast< bmp_image_width::type  >( read_int16() );
            _info._height = static_cast< bmp_image_height::type >( read_int16() );

            // the number of color planes being used. Must be set to 1.
            read_int16();

            _info._bits_per_pixel = read_int16();

            _info._compression = rgb;

            _info._image_size = read_int32();

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

        _info.valid = true;

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


    }

private:

    Device& _io_dev;
    image_read_info< bmp_tag > _info;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED
