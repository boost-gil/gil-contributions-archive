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
/// \date 2008 \n
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
    {}

    Device                     _io_dev;

    image_read_settings< bmp_tag > _settings;
    image_read_info< bmp_tag >     _info;

    std::vector< rgba8_pixel_t > _palette;
};


template< typename Device
        , typename ConversionPolicy
        , typename View
        >
class reader< Device
            , bmp_tag
            , ConversionPolicy
            , View
            >
    : /*public detail::reader_base< bmp_tag
                                , ConversionPolicy
                                >
    , */public reader_backend< Device
                           , bmp_tag
                           >
{
private:

    typedef reader< Device, bmp_tag, ConversionPolicy, View > this_t;

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    //
    // Constructor
    //
    reader( Device&                               device
          , const image_read_settings< bmp_tag >& settings
          )
    : /*reader_base< bmp_tag
                 , ConversionPolicy >( settings )

    , */reader_backend( device, settings )

    , _pitch( 0 )
    , _offset( 0 )
    , _ybeg( 0 )
    , _yend( 0 )
    , _yinc( 0 )
    {}

    //
    // Constructor
    //
    reader( Device&                               device
          , const cc_t&                           cc
          , const image_read_settings< bmp_tag >& settings
          )
    : /*reader_base< bmp_tag
                 , ConversionPolicy
                 >( cc
                  , settings
                  )
    ,*/ reader_backend( device, settings )
    , _pitch( 0 )
    , _offset( 0 )
    , _ybeg( 0 )
    , _yend( 0 )
    , _yinc( 0 )
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

    void check_destination_view( const View& dst_view )
    {
        typedef point_t::value_type int_t;

        int_t width  = static_cast< int_t >( _info._width  );
        int_t height = static_cast< int_t >( _info._height );

        io_error_if( (  ( width  ) <= dst_view.width()
                     || ( height ) <= dst_view.height()
                     )
                  , "User provided view has incorrect size." 
                  );
    }

    void initialize()
    {
        // make sure the dst view is compatible with bmp format
        typedef typename is_same< ConversionPolicy
                                , read_and_no_convert
                                >::type is_read_and_convert_t;

        io_error_if( !is_allowed< View >( this->_info
                                        , is_read_and_convert_t()
                                        )
                   , "Image types aren't compatible."
                   );

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
        _ybeg = 0;
        _yend = this->_settings._dim.y;
        _yinc = 1;

        if( _info._height > 0 )
        {
            // the image is upside down
            _ybeg = this->_settings._dim.y - 1;
            _yend = -1;
            _yinc = -1;

            _offset = _info._offset
                    + (   this->_info._height
                        - this->_settings._top_left.y
                        - this->_settings._dim.y
                      ) * _pitch;


        }
        else
        {
            _offset = _info._offset
                    + this->_settings._top_left.y * _pitch;
        }


        switch( _info._bits_per_pixel )
        {
            case 1: { _read_function = boost::mem_fn( &this_t::read_1_bit_image ); break; }

            case 4:
            {
				switch( _info._compression )
				{
				    case bmp_compression::_rle4: { _read_function = boost::mem_fn( &this_t::read_rle_image );    break; }
				    case bmp_compression::_rgb : { _read_function = boost::mem_fn( &this_t::read_4_bits_image ); break; }

				    default: { io_error( "Unsupported compression mode in BMP file." ); break; }
                }

                break;
            }

            case 8:
            {
				switch( _info._compression )
				{
				    case bmp_compression::_rle8: { _read_function = boost::mem_fn( &this_t::read_rle_image );    break; }
				    case bmp_compression::_rgb:  { _read_function = boost::mem_fn( &this_t::read_8_bits_image ); break; }

				    default: { io_error( "Unsupported compression mode in BMP file." ); break; }
                }

                break;
            }

            case 15: case 16: { _read_function = boost::mem_fn( &this_t::read_15_bits_image ); break; }

            case 24: { _read_function = boost::mem_fn( &this_t::read_24_bits_image ); break; }
            case 32: { _read_function = boost::mem_fn( &this_t::read_32_bits_image ); break; }

            default: { io_error( "Unsupported bits per pixel." ); break; }
        }
    }

    /// Read part of image defined by View and return the data.
    View read( View dst )
    {
        _read_function(this, dst);

        return dst;
    }

private:

    void read_palette()
    {
        if( _palette.size() > 0 )
        {
            // palette has been read already.
            return;
        }

        int entries = _info._num_colors;

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

    // Read 1 bit image. The colors are encoded by an index.
    void read_1_bit_image( View dst )
    {
        typedef gray1_image_t::view_t src_view_t;

        read_palette();
    }

    // Read 4 bits image. The colors are encoded by an index.
    void read_4_bits_image( View dst )
    {
        typedef gray4_image_t::view_t src_view_t;

        read_palette();
    }    

    /// Read 8 bits image. The colors are encoded by an index.
    void read_8_bits_image( View dst )
    {
        typedef gray8_image_t::view_t src_view_t;

        read_palette();
    }    

    /// Read image that's encoded using Run-length coding (RLE).
    void read_rle_image( View dst )
    {
        assert(  _info._compression == bmp_compression::_rle4
              || _info._compression == bmp_compression::_rle8
              );

        read_palette();
    }

    /// Read 15 or 16 bits image.
    void read_15_bits_image( View dst )
    {
        
    }

    /// Read 24 bits image.
    void read_24_bits_image( View dst )
    {
        typedef bgr24_image_t::view_t src_view_t;
    }

    /// Read 32 bits image.
    void read_32_bits_image( View dst )
    {
        typedef bgra24_image_t::view_t src_view_t;
    }

private:

    // the row pitch must be multiple of 4 bytes
    int _pitch;

    // offset to first scanline
    std::ptrdiff_t _offset;

    std::ptrdiff_t _ybeg;
    std::ptrdiff_t _yend;
    std::ptrdiff_t _yinc;

    boost::function< void ( this_t*, View ) > _read_function;
};

/////////////////////////////////// dynamic image

class bmp_type_format_checker
{
public:

    bmp_type_format_checker( const bmp_bits_per_pixel::type& bpp )
    : _bpp( bpp )
    {}

    template< typename Image >
    bool apply()
    {
        if( _bpp < 32 )
        {
            return pixels_are_compatible< typename Image::value_type, rgb8_pixel_t >::value
                   ? true
                   : false;
        }
        else
        {
            return pixels_are_compatible< typename Image::value_type, rgba8_pixel_t >::value
                   ? true
                   : false;
        }
    }

private:

    const bmp_bits_per_pixel::type& _bpp;
};

struct bmp_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , bmp_tag
                                           >
    {};
};

template< typename Device
        , typename View
        >
class dynamic_image_reader< Device
                          , bmp_tag
                          , View
                          >
    : public reader< Device
                   , bmp_tag
                   , read_and_no_convert
                   , View
                   >
{
    typedef reader< Device
                  , bmp_tag
                  , read_and_no_convert
                  , View
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< bmp_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        if( !this->_info._valid )
        {
            parent_t::get_info();
        }

        bmp_type_format_checker format_checker( this->_info._bits_per_pixel );

        if( !construct_matched( images
                              , format_checker
                              ))
        {
            io_error( "No matching image type between those of the given any_image and that of the file" );
        }
        else
        {
            init_image( images
                      , this->_info
                      );

            detail::dynamic_io_fnobj< bmp_read_is_supported
                                    , parent_t
                                    > op( this );

            apply_operation( view( images )
                           , op
                           );
        }
    }
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP
