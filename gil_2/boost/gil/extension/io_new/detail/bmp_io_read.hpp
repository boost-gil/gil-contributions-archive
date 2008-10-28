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
#include <boost/gil/extension/io_new/bmp_tags.hpp>

#include "base.hpp"
#include "reader_base.hpp"
#include "io_device.hpp"
#include "typedefs.hpp"
#include "bmp_supported_types.hpp"

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
	    ++n;
    }

    return n;
}

inline
void swap_bits_( unsigned char& c )
{
   unsigned char result = 0;
   for( int i = 0; i < 8; ++i )
   {
      result = result << 1;
      result |= ( c & 1 );
      c = c >> 1;
   }

   c = result;
}

inline
void swap_half_bytes( unsigned char& c )
{
    unsigned char b = ( c & 0xF ) << 4;
    c = ( c >> 4 ) | b;
}


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
    : public reader_base< bmp_tag
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
        if( _io_dev.read_int16() == 0x424D )
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
    void apply( const View& dst_view )
    {
        if( !_info._valid )
        {
            get_info();
        }

        // read the color masks
        color_mask mask;
        if( _info._compression == ct_bitfield )
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
        else if( _info._compression == ct_rgb )
        {
            switch( _info._bits_per_pixel )
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
        std::vector< rgba8_pixel_t > palette;

        if( _info._bits_per_pixel <= 8 )
        {
            int entries = _info._num_colors;

            if( entries == 0 )
            {
                entries = 1 << _info._bits_per_pixel;
            }

            palette.resize( entries );

            for( int i = 0; i < entries; ++i )
            {
                get_color( palette[i], blue_t()  ) = _io_dev.read_int8();
                get_color( palette[i], green_t() ) = _io_dev.read_int8();
                get_color( palette[i], red_t()   ) = _io_dev.read_int8();

                // there are 4 entries when windows header
                // but 3 for os2 header
                if( _info._header_size == win32_info_size )
                {
                    _io_dev.read_int8();
                }

            } // for
        } // if


        // the row pitch must be multiple 4 bytes
        int pitch;

        if( _info._bits_per_pixel < 8 )
        {
            pitch = (( this->_info._width * this->_info._bits_per_pixel ) + 7 ) >> 3;
        }
        else
        {
            pitch = _info._width * (( this->_info._bits_per_pixel + 7 ) >> 3);
        }

        pitch = (pitch + 3) & ~3;

        // read the raster
        std::vector< byte_t > row( pitch );

        int ybeg = 0;
        int yend = this->_settings._dim.y;
        int yinc = 1;

        if( _info._height > 0 )
        {
            // the image is upside down
            ybeg = this->_settings._dim.y - 1;
            yend = -1;
            yinc = -1;

            // go to the first scanline to read
            _io_dev.seek( _info._offset
                        + (   this->_info._height 
                            - this->_settings._top_left.y 
                            - this->_settings._dim.y 
                          ) * pitch
                        );

        }
        else
        {
            // go to the first scanline to read
            _io_dev.seek( _info._offset
                        + this->_settings._top_left.y * pitch
                        );

        }

        for( int y = ybeg; y != yend; y += yinc )
        {
            // @todo: For now we're reading the whole scanline which is
            // slightly inefficient. Later versions should try to read
            // only the bytes which are necessary.
            _io_dev.read( &row.front(), pitch );

            switch( _info._bits_per_pixel )
            {
                case 1:
                {
                    // 2-bit indices

                    // we have to swap bits
                    // 11101100 -> 00110111
                    for_each( row.begin(), row.end(), swap_bits_ );

                    // row contains the indices
                    typedef bit_aligned_image1_type< 1, gray_layout_t >::type image_t;
                    typedef image_t::view_t::x_iterator it_t;
                    
                    it_t it( &row.front(), 0 );
                    it += this->_settings._top_left.x;
                    it_t end = it + this->_settings._dim.x;

                    typename View::x_iterator dst_it = dst_view.row_begin( y );

                    for( ; it != end; ++it, ++dst_it )
                    {
                        unsigned char c = get_color( *it, gray_color_t() );
                        *dst_it = palette[ c ];
                    }

                    break;
                }

                case 4:
                {
                    // 4-bit indices

                    // we have to swap half bytes
                    // 11101100 -> 11001110
                    for_each( row.begin(), row.end(), swap_half_bytes );

                    // row contains the indices
                    typedef bit_aligned_image1_type< 4, gray_layout_t >::type image_t;
                    typedef image_t::view_t::x_iterator it_t;

                    it_t it( &row.front(), 0 );
                    it += this->_settings._top_left.x;
                    it_t end = it + this->_settings._dim.x;

                    typename View::x_iterator dst_it = dst_view.row_begin( y );

                    for( ; it != end; ++it, ++dst_it )
                    {
                        unsigned char c = get_color( *it, gray_color_t() );
                        *dst_it = palette[ c ];
                    }

                    break;
                }

                case 8:
                {
                    // 8-bit indices
                    typedef gray8_image_t image_t;

                    gray8_view_t v = interleaved_view( _info._width
                                                     , 1
                                                     , (gray8_pixel_t*) &row.front()
                                                     , _info._width
                                                     );

                    gray8_view_t::x_iterator it  = v.row_begin( 0 ) + this->_settings._top_left.x;
                    gray8_view_t::x_iterator end = it + this->_settings._dim.x;


                    typename View::x_iterator dst_it = dst_view.row_begin( y );

                    for( ; it != end; ++it, ++dst_it )
                    {
                        unsigned char c = get_color( *it, gray_color_t() );
                        *dst_it = palette[ c ];
                    }


                    break;
                }

                case 15:
                case 16:
                {
                    typedef rgb8_image_t image_t;
                    typedef image_t::view_t::x_iterator it_t;

                    image_t img_row( _info._width, 1 );
                    image_t::view_t v = view( img_row );
                    it_t it = v.row_begin( 0 );

                    unsigned char* src = &row.front();
                    for( int i = 0 ; i < _info._width; ++i, src += 2 )
                    {
				        int p = ( src[1] << 8 ) | src[0];

				        int r = ((p & mask.red.mask)   >> mask.red.shift)   << (8 - mask.red.width);
				        int g = ((p & mask.green.mask) >> mask.green.shift) << (8 - mask.green.width);
				        int b = ((p & mask.blue.mask)  >> mask.blue.shift)  << (8 - mask.blue.width);

                        get_color( it[i], red_t()   ) = r;
                        get_color( it[i], green_t() ) = g;
                        get_color( it[i], blue_t()  ) = b;
                    }

                    it_t beg = v.row_begin( 0 ) + this->_settings._top_left.x;
                    it_t end = beg + this->_settings._dim.x;

                    this->_cc_policy.read( beg
                                         , end
                                         , dst_view.row_begin( y )
                                         );

                    break;
                }

                case 24:
                {
                    // 8-8-8 BGR
                    bgr8_view_t v = interleaved_view( _info._width
                                                    , 1
                                                    , (bgr8_pixel_t*) &row.front()
                                                    , _info._width * 3
                                                    );

                    bgr8_view_t::x_iterator beg = v.row_begin( 0 ) + this->_settings._top_left.x;
                    bgr8_view_t::x_iterator end = beg + this->_settings._dim.x;

                    this->_cc_policy.read( beg
                                         , end
                                         , dst_view.row_begin( y )
                                         );

                    break;
                }


                case 32:
                {
                    // 8-8-8-8 BGRA
                    bgra8_view_t v = interleaved_view( _info._width
                                                     , 1
                                                     , (bgra8_pixel_t*) &row.front()
                                                     , _info._width * 4
                                                     );

                    bgra8_view_t::x_iterator beg = v.row_begin( 0 ) + this->_settings._top_left.x;
                    bgra8_view_t::x_iterator end = beg + this->_settings._dim.x;

                    this->_cc_policy.read( beg
                                         , end
                                         , dst_view.row_begin( y )
                                         );

                    break;
                }
            }
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
