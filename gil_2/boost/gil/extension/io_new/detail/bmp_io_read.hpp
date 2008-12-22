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
#include "bit_operations.hpp"
#include "row_buffer_helper.hpp"
#include "reader_base.hpp"
#include "io_device.hpp"
#include "typedefs.hpp"
#include "bmp_supported_types.hpp"

namespace boost { namespace gil { namespace detail {

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

        if( _info._header_size == bmp_win32_info_size )
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
        else if( _info._header_size == bmp_os2_info_size )
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


        int ybeg = 0;
        int yend = this->_settings._dim.y;
        int yinc = 1;

        // offset to first scanline
        int offset = 0;

        if( _info._height > 0 )
        {
            // the image is upside down
            ybeg = this->_settings._dim.y - 1;
            yend = -1;
            yinc = -1;

            offset = _info._offset
                   + (   this->_info._height 
                       - this->_settings._top_left.y 
                       - this->_settings._dim.y 
                     ) * pitch;


        }
        else
        {
            offset = _info._offset
                   + this->_settings._top_left.y * pitch;
        }

        switch( _info._bits_per_pixel )
        {
            case 1:
            {
                read_palette_image< gray1_image_t::view_t
                                  , mirror_bits< byte_vector_t, mpl::true_ >
                                  > ( dst_view, pitch, ybeg, yend, yinc, offset ); 
                break;
            }

            case 4:
            {
                read_palette_image< gray4_image_t::view_t
                                  , swap_half_bytes< byte_vector_t, mpl::true_ >
                                  > ( dst_view, pitch, ybeg, yend, yinc, offset ); 
                break;
            }

            case 8:
            {
                read_palette_image< gray8_image_t::view_t
                                  , do_nothing< std::vector< gray8_pixel_t > >
                                  > ( dst_view, pitch, ybeg, yend, yinc, offset ); 
                break;
            }

            case 15: case 16: {  read_data_15( dst_view, pitch, ybeg, yend, yinc, offset  ); break; }

            case 24: {  read_data< bgr8_view_t  >( dst_view, pitch, ybeg, yend, yinc, offset  ); break; }
            case 32: {  read_data< bgra8_view_t >( dst_view, pitch, ybeg, yend, yinc, offset  ); break; }

        }
    }

private:

    void read_palette( std::vector< rgba8_pixel_t >& palette )
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
            if( _info._header_size == bmp_win32_info_size )
            {
                _io_dev.read_int8();
            }

        } // for
    }

    template< typename View_Src
            , typename Byte_Manipulator
            , typename View_Dst
            >
    void read_palette_image( const View_Dst& view
                           , int             pitch
                           , int             ybeg
                           , int             yend
                           , int             yinc
                           , int             offset
                           )
    {
        std::vector< rgba8_pixel_t > pal;
        read_palette( pal );

        // jump to first scanline
        _io_dev.seek( offset );

        typedef row_buffer_helper_view< View_Src > rh_t;
        typedef typename rh_t::iterator_t          it_t;

        rh_t rh( pitch, true );

        // we have to swap bits
        Byte_Manipulator byte_manipulator;

        for( int y = ybeg; y != yend; y += yinc )
        {
            // @todo: For now we're reading the whole scanline which is
            // slightly inefficient. Later versions should try to read
            // only the bytes which are necessary.
            _io_dev.read( reinterpret_cast< byte_t* >( rh.data() )
                        , pitch
                        );

            byte_manipulator( rh.buffer() );

            typename View_Dst::x_iterator dst_it = view.row_begin( y );

            it_t it  = rh.begin() + this->_settings._top_left.x;
            it_t end = it + this->_settings._dim.x;

            for( ; it != end; ++it, ++dst_it )
            {
                unsigned char c = get_color( *it, gray_color_t() );
                *dst_it = pal[ c ];
            }
        }
    }

    template< typename View >
    void read_data_15( const View& view
                     , int         pitch
                     , int         ybeg
                     , int         yend
                     , int         yinc
                     , int         offset
                     )
    {
        byte_vector_t row( pitch );

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

        // jump to first scanline
        _io_dev.seek( offset );

        typedef rgb8_image_t image_t;
        typedef image_t::view_t::x_iterator it_t;

        for( int y = ybeg; y != yend; y += yinc )
        {
            // @todo: For now we're reading the whole scanline which is
            // slightly inefficient. Later versions should try to read
            // only the bytes which are necessary.
            _io_dev.read( &row.front(), row.size() );

            image_t img_row( _info._width, 1 );
            image_t::view_t v = gil::view( img_row );
            it_t it = v.row_begin( 0 );

            it_t beg = v.row_begin( 0 ) + this->_settings._top_left.x;
            it_t end = beg + this->_settings._dim.x;

            byte_t* src = &row.front();
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

            this->_cc_policy.read( beg
                                 , end
                                 , view.row_begin( y )
                                 );
        }
    }


    // 8-8-8 BGR
    // 8-8-8-8 BGRA
    template< typename View_Src
            , typename View_Dst
            >
    void read_data( const View_Dst& view
                  , int             pitch
                  , int             ybeg
                  , int             yend
                  , int             yinc
                  , int             offset
                  )
    {
        byte_vector_t row( pitch );

        // jump to first scanline
        _io_dev.seek( offset );

        View_Src v = interleaved_view( _info._width
                                     , 1
                                     , (typename View_Src::value_type*) &row.front()
                                     , _info._width * num_channels< View_Src >::value
                                     );

        typename View_Src::x_iterator beg = v.row_begin( 0 ) + this->_settings._top_left.x;
        typename View_Src::x_iterator end = beg + this->_settings._dim.x;

        for( int y = ybeg; y != yend; y += yinc )
        {
            // @todo: For now we're reading the whole scanline which is
            // slightly inefficient. Later versions should try to read
            // only the bytes which are necessary.
            _io_dev.read( &row.front(), row.size() );

            this->_cc_policy.read( beg
                                 , end
                                 , view.row_begin( y )
                                 );
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
