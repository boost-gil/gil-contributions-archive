/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2008 - 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/function.hpp>

#include <boost/gil/extension/io_new/bmp_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "is_allowed.hpp"

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
    , _palette()
    {}

    ~reader_backend()
    {
        _io_dev.set_close( true );
    }

    Device _io_dev;

    image_read_settings< bmp_tag > _settings;
    image_read_info< bmp_tag >     _info;

    std::vector< rgba8_pixel_t > _palette;

    color_mask _mask;
};

///
/// BMP Reader
///
template< typename Device >
class scanline_reader< Device
                     , bmp_tag
                     >
    : public reader_backend< Device
                           , bmp_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , bmp_tag
                           > this_t;

public:

    typedef reader_backend< Device, bmp_tag > backend_t;

public:

    //
    // Constructor
    //
    scanline_reader( Device&                               device
                   , const image_read_settings< bmp_tag >& settings
                   )
    : reader_backend( device, settings )

    , _pitch( 0 )
    , _scanline_length( 0 )
    {}

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

    void initialize()
    {
        if( _info._bits_per_pixel < 8 )
        {
            _pitch = (( this->_info._width * this->_info._bits_per_pixel ) + 7 ) >> 3;
        }
        else
        {
            _pitch = _info._width * (( this->_info._bits_per_pixel + 7 ) >> 3);
        }

        _pitch = (_pitch + 3) & ~3;

        //

        switch( _info._bits_per_pixel )
        {
            case 1:
            {
                _scanline_length = ( this->_info._width * num_channels< rgba8_view_t >::value + 3 ) & ~3;

                read_palette();
                _buffer.resize( _pitch );

                _read_function = boost::mem_fn( &this_t::read_1_bit_row );

                break;
            }

            case 4:
            {
				switch( _info._compression )
				{
				    case bmp_compression::_rle4:
                    {
                        io_error( "Cannot read run-length encoded images in iterator mode. Try to read as whole image." );

                        break;
                    }

				    case bmp_compression::_rgb :
                    {
                        _scanline_length = ( this->_info._width * num_channels< rgba8_view_t >::value + 3 ) & ~3;

                        read_palette();
                        _buffer.resize( _pitch );

                        _read_function = boost::mem_fn( &this_t::read_4_bits_row );

                        break;
                    }

				    default:
                    {
                        io_error( "Unsupported compression mode in BMP file." );
                    }
                }

                break;
            }

            case 8:
            {
				switch( _info._compression )
				{
				    case bmp_compression::_rle8:
                    {
                        io_error( "Cannot read run-length encoded images in iterator mode. Try to read as whole image." );

                        break;
                    }
				    case bmp_compression::_rgb:
                    {
                        _scanline_length = ( this->_info._width * num_channels< rgba8_view_t >::value + 3 ) & ~3;

                        read_palette();
                        _buffer.resize( _pitch );

                        _read_function = boost::mem_fn( &this_t::read_8_bits_row ); 

                        break;
                    }

				    default: { io_error( "Unsupported compression mode in BMP file." ); break; }
                }

                break;
            }

            case 15:
            case 16:
            {
                _scanline_length = ( this->_info._width * num_channels< rgb8_view_t >::value + 3 ) & ~3;

                _buffer.resize( _pitch );

                if( _info._compression == bmp_compression::_bitfield )
                {
                    _mask.red.mask    = _io_dev.read_uint32();
                    _mask.green.mask  = _io_dev.read_uint32();
                    _mask.blue.mask   = _io_dev.read_uint32();

                    _mask.red.width   = detail::count_ones( _mask.red.mask   );
                    _mask.green.width = detail::count_ones( _mask.green.mask );
                    _mask.blue.width  = detail::count_ones( _mask.blue.mask  );

                    _mask.red.shift   = detail::trailing_zeros( _mask.red.mask   );
                    _mask.green.shift = detail::trailing_zeros( _mask.green.mask );
                    _mask.blue.shift  = detail::trailing_zeros( _mask.blue.mask  );
                }
                else if( _info._compression == bmp_compression::_rgb )
                {
                    switch( _info._bits_per_pixel )
                    {
                        case 15:
                        case 16:
                        {
                            _mask.red.mask   = 0x007C00; _mask.red.width   = 5; _mask.red.shift   = 10;
                            _mask.green.mask = 0x0003E0; _mask.green.width = 5; _mask.green.shift =  5;
                            _mask.blue.mask  = 0x00001F; _mask.blue.width  = 5; _mask.blue.shift  =  0;

                            break;
                        }

                        case 24:
                        case 32:
                        {
                            _mask.red.mask   = 0xFF0000; _mask.red.width   = 8; _mask.red.shift   = 16;
                            _mask.green.mask = 0x00FF00; _mask.green.width = 8; _mask.green.shift =  8;
                            _mask.blue.mask  = 0x0000FF; _mask.blue.width  = 8; _mask.blue.shift  =  0;

                            break;
                        }
                    }
                }
                else
                {
                    io_error( "Unsupported BMP compression." );
                }


                _read_function = boost::mem_fn( &this_t::read_15_bits_row ); 

                break;
            }

            case 24:
            {
                _scanline_length = ( this->_info._width * num_channels< rgb8_view_t >::value + 3 ) & ~3;
                _read_function = boost::mem_fn( &this_t::read_row ); 

                break;
            }

            case 32:
            {
                _scanline_length = ( this->_info._width * num_channels< rgba8_view_t >::value + 3 ) & ~3;
                _read_function = boost::mem_fn( &this_t::read_row ); 
                
                break;
            }

            default:
            {
                io_error( "Unsupported bits per pixel." );
            }
        }
    }

    void clean_up(){}

    /// Read part of image defined by View and return the data.
    void read( byte_t* dst, int pos )
    {
        // jump to scanline
        long offset = 0;

        if( _info._height > 0 )
        {
            // the image is upside down
            offset = _info._offset
                   + ( this->_info._height - 1 - pos ) * this->_pitch;
        }
        else
        {
            offset = _info._offset
                   + pos * _pitch;
        }
        
        _io_dev.seek( offset );


        // read data
        _read_function(this, dst);
    }

    /// Skip over a scanline.
    void skip( byte_t*, int )
    {
        // nothing to do.
    }

    /// Return length of scanline in bytes.
    std::size_t scanline_length()
    {
        return _scanline_length;
    }

private:

    void read_palette()
    {
        if( _palette.size() > 0 )
        {
            // palette has been read already.
            return;
        }

        int entries = this->_info._num_colors;

        if( entries == 0 )
        {
            entries = 1 << _info._bits_per_pixel;
        }

        _palette.resize( entries );

        for( int i = 0; i < entries; ++i )
        {
            get_color( _palette[i], blue_t()  ) = _io_dev.read_uint8();
            get_color( _palette[i], green_t() ) = _io_dev.read_uint8();
            get_color( _palette[i], red_t()   ) = _io_dev.read_uint8();

            // there are 4 entries when windows header
            // but 3 for os2 header
            if( _info._header_size == bmp_header_size::_win32_info_size )
            {
                _io_dev.read_uint8();
            }

        } // for
    }

    template< typename View >
    void read_bit_row( byte_t* dst )
    {
        typedef View src_view_t;
        typedef rgba8_image_t::view_t dst_view_t;
        
        src_view_t src_view = interleaved_view( this->_info._width
                                              , 1
                                              , (src_view_t::x_iterator) &_buffer.front()
                                              , this->_pitch
                                              );

        dst_view_t dst_view = interleaved_view( this->_info._width
                                              , 1
                                              , (dst_view_t::value_type*) dst
                                              , num_channels< dst_view_t >::value * this->_info._width
                                              );
        

        src_view_t::x_iterator src_it = src_view.row_begin( 0 );
        dst_view_t::x_iterator dst_it = dst_view.row_begin( 0 );

        for( dst_view_t::x_coord_t i = 0
           ; i < _info._width
           ; ++i, src_it++, dst_it++
           )
        {
            unsigned char c = get_color( *src_it, gray_color_t() );
            *dst_it = this->_palette[c];
        }
    }

    // Read 1 bit image. The colors are encoded by an index.
    void read_1_bit_row( byte_t* dst )
    {
        assert( _io_dev.read( &_buffer.front(), _pitch ) );
        _mirror_bits( _buffer );

        read_bit_row< gray1_image_t::view_t >( dst );
    }

    // Read 4 bits image. The colors are encoded by an index.
    void read_4_bits_row( byte_t* dst )
    {
        assert( _io_dev.read( &_buffer.front(), _pitch ) );
        _swap_half_bytes( _buffer );

        read_bit_row< gray4_image_t::view_t >( dst );
    }

    /// Read 8 bits image. The colors are encoded by an index.
    void read_8_bits_row( byte_t* dst )
    {
        assert( _io_dev.read( &_buffer.front(), _pitch ) );

        read_bit_row< gray8_image_t::view_t >( dst );
    }

    /// Read 15 or 16 bits image.
    void read_15_bits_row( byte_t* dst )
    {
        typedef rgb8_view_t dst_view_t;
        typedef dst_view_t::x_iterator it_t;

        dst_view_t dst_view = interleaved_view( this->_info._width
                                              , 1
                                              , (dst_view_t::value_type*) dst
                                              , this->_pitch
                                              );

        dst_view_t::x_iterator dst_it = dst_view.row_begin( 0 );

        //
        byte_t* src = &_buffer.front();
        _io_dev.read( src, _pitch );

        for( dst_view_t::x_coord_t i = 0
           ; i < _info._width
           ; ++i, src += 2
           )
        {
            int p = ( src[1] << 8 ) | src[0];

            int r = ((p & _mask.red.mask)   >> _mask.red.shift)   << (8 - _mask.red.width);
            int g = ((p & _mask.green.mask) >> _mask.green.shift) << (8 - _mask.green.width);
            int b = ((p & _mask.blue.mask)  >> _mask.blue.shift)  << (8 - _mask.blue.width);

            get_color( dst_it[i], red_t()   ) = static_cast< byte_t >( r );
            get_color( dst_it[i], green_t() ) = static_cast< byte_t >( g );
            get_color( dst_it[i], blue_t()  ) = static_cast< byte_t >( b );
        }
    }

    void read_row( byte_t* dst )
    {
        assert( _io_dev.read( dst, _pitch ) );
    }

private:

    // the row pitch must be multiple of 4 bytes
    int _pitch;

    int _scanline_length;

    std::vector< byte_t > _buffer;
    detail::mirror_bits    < std::vector< byte_t >, mpl::true_ > _mirror_bits;
    detail::swap_half_bytes< std::vector< byte_t >, mpl::true_ > _swap_half_bytes;

    boost::function< void ( this_t*, byte_t* ) > _read_function;
    boost::function< void ( this_t* )          > _skip_function;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP
