/*
    Copyright 2007-2012 Christian Henning, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

// taken from jpegxx - https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/ijg_headers.hpp
#ifndef BOOST_GIL_EXTENSION_IO_TIFF_C_LIB_COMPILED_AS_CPLUSPLUS
    extern "C" {
#else
    // DONT_USE_EXTERN_C introduced in v7 of the IJG library.
    // By default the v7 IJG headers check for __cplusplus being defined and
    // wrap the content in an 'extern "C"' block if it's present.
    // When DONT_USE_EXTERN_C is defined, this wrapping is not performed.
    #ifndef DONT_USE_EXTERN_C
        #define DONT_USE_EXTERN_C 1
    #endif
#endif

#include <tiff.h>
#include <tiffio.h>

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_C_LIB_COMPILED_AS_CPLUSPLUS
    }
#endif

#include <algorithm>
#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/static_assert.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>

#include "device.hpp"
#include "is_allowed.hpp"


namespace boost { namespace gil {

///
/// TIFF Backend
///
template< typename Device >
struct reader_backend< Device
                     , tiff_tag
                     >
{
    reader_backend( Device&                                device
                  , const image_read_settings< tiff_tag >& settings
                  )
    : _io_dev  ( device   )
    , _settings( settings )
    , _info()

    , _scanline_length( 0 )

    , _red  ( NULL )
    , _green( NULL )
    , _blue ( NULL )
    {
        init_multipage_read( settings );
    }

    ~reader_backend()
    {
        ///@todo
        //_io_dev.set_close( true );
    }

private:

    void init_multipage_read( const image_read_settings< tiff_tag >& settings )
    {
        if( settings._directory > 0 )
        {
            _io_dev.set_directory( settings._directory );
        }
    } 

public:

    Device _io_dev;

    image_read_settings< tiff_tag > _settings;
    image_read_info< tiff_tag >     _info;

    std::size_t _scanline_length;

    // palette
    tiff_color_map::red_t   _red;
    tiff_color_map::green_t _green;
    tiff_color_map::blue_t  _blue;

    rgb16_planar_view_t _palette;
};

///
/// TIFF scanline reader
///

template< typename Device >
class scanline_reader< Device
                     , tiff_tag
                     >
    : public reader_backend< Device
                           , tiff_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , tiff_tag
                           > this_t;
public:

    typedef reader_backend< Device
                          , tiff_tag
                          > backend_t;

public:

    scanline_reader( Device&                                device
                   , const image_read_settings< tiff_tag >& settings
                   )
    : backend_t( device
               , settings
               )
    {}

   void read_header()
   {
      io_error_if( _io_dev.template get_property<tiff_image_width>               ( _info._width ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_image_height>              ( _info._height ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_compression>               ( _info._compression ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_samples_per_pixel>         ( _info._samples_per_pixel ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_bits_per_sample>           ( _info._bits_per_sample ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_sample_format>             ( _info._sample_format ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_planar_configuration>      ( _info._planar_configuration ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_photometric_interpretation>( _info._photometric_interpretation  ) == false
                 , "cannot read tiff tag." );

      _info._is_tiled = false;

      // Tile tags
      if( _io_dev.is_tiled() )
      {
          _info._is_tiled = true;

          io_error_if( !_io_dev.template get_property< tiff_tile_width  >( _info._tile_width )
                       , "cannot read tiff_tile_width tag." );
          io_error_if( !_io_dev.template get_property< tiff_tile_length >( _info._tile_length )
                       , "cannot read tiff_tile_length tag." );
      }
   }

    void initialize()
    {
        io_error_if( _info._is_tiled
                   , "scanline_reader doesn't support tiled tiff images."
                   );

        if( this->_info._photometric_interpretation == PHOTOMETRIC_PALETTE )
        {

            this->_scanline_length = this->_info._width 
                                   * num_channels< rgb16_view_t >::value 
                                   * sizeof( channel_type<rgb16_view_t>::type );

            _io_dev.get_field_defaulted( this->_red
                                        , this->_green
                                        , this->_blue
                                        );

            _buffer = std::vector< byte_t >( _io_dev.get_scanline_size() );

            switch( this->_info._bits_per_sample )
            {
                case 1:
                {
                    typedef channel_type< get_pixel_type< gray1_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_1_bit_index_image );

                    break;
                }

                case 2:
                {
                    typedef channel_type< get_pixel_type< gray2_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_2_bits_index_image );

                    break;
                }
                case 4:
                {
                    typedef channel_type< get_pixel_type< gray4_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_4_bits_index_image );

                    break;
                }

                case 8:  
                {
                    typedef channel_type< get_pixel_type< gray8_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_8_bits_index_image );

                    break;
                }

                case 16:
                {
                    typedef channel_type< get_pixel_type< gray16_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_16_bits_index_image );

                    break;
                }

                case 24:
                {
                    typedef channel_type< get_pixel_type< gray24_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_24_bits_index_image );

                    break;
                }

                case 32:
                {
                    typedef channel_type< get_pixel_type< gray32_image_t::view_t >::type >::type channel_t;

                    int num_colors = channel_traits< channel_t >::max_value() + 1;

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , this->_red
                                              , this->_green
                                              , this->_blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_32_bits_index_image );

                    break;
                }
                default: { io_error( "Not supported palette " ); }
            }
        }
        else
        {
            if( this->_info._planar_configuration == PLANARCONFIG_SEPARATE )
            {
                io_error( "scanline_reader doesn't support planar tiff images." );
            }
            else if( this->_info._planar_configuration == PLANARCONFIG_CONTIG )
            {
                this->_scanline_length = _io_dev.get_scanline_size();

                // the read_data function needs to know what gil type the source image is
                // to have the default color converter function correctly

                switch( this->_info._photometric_interpretation )
                {
                    case PHOTOMETRIC_MINISWHITE:
                    case PHOTOMETRIC_MINISBLACK:
                    {
                        switch( this->_info._bits_per_sample )
                        {
                            case  1: 
                            case  2: 
                            case  4:
                            case  6: 
                            case  8: 
                            case 10: 
                            case 12: 
                            case 14: 
                            case 16: 
                            case 24: 
                            case 32: { _read_function = boost::mem_fn( &this_t::read_row ); break; }
                            default: { io_error( "Image type is not supported." ); }
                        }

                        break;
                    }

                    case PHOTOMETRIC_RGB:
                    {
                        switch( this->_info._samples_per_pixel )
                        {
                            case 3:
                            {
                                switch( this->_info._bits_per_sample )
                                {
                                    case  2: 
                                    case  4: 
                                    case  8: 
                                    case 10: 
                                    case 12: 
                                    case 14: 
                                    case 16: 
                                    case 24: 
                                    case 32: { _read_function = boost::mem_fn( &this_t::read_row );  break; }
                                    default: { io_error( "Image type is not supported." ); }
                                }

                                break;
                            }

                            case 4:
                            {
                                switch( this->_info._bits_per_sample )
                                {
                                    case  2: 
                                    case  4: 
                                    case  8: 
                                    case 10: 
                                    case 12: 
                                    case 14: 
                                    case 16: 
                                    case 24: 
                                    case 32: { _read_function = boost::mem_fn( &this_t::read_row );  break; }
                                    default: { io_error( "Image type is not supported." ); }
                                }

                                break;
                            }

                            default: { io_error( "Image type is not supported." ); }
                        }

                        break;
                    }
                    case PHOTOMETRIC_SEPARATED: // CYMK
                    {
                        switch( this->_info._bits_per_sample )
                        {
                            case  2: 
                            case  4: 
                            case  8: 
                            case 10: 
                            case 12: 
                            case 14: 
                            case 16: 
                            case 24: 
                            case 32: { _read_function = boost::mem_fn( &this_t::read_row );  break; }
                            default: { io_error( "Image type is not supported." ); }
                        }

                        break;
                    }

                    default: { io_error( "Image type is not supported." ); }
                }
            }
            else
            {
                io_error( "Wrong planar configuration setting." );
            }
        }
    }

    /// Read part of image defined by View and return the data.
    void read( byte_t* dst, int pos )
    {
        _read_function( this, dst, pos );
    }    

    /// Skip over a scanline.
    void skip( byte_t* dst, int pos )
    {
        this->_read_function( this, dst, pos );
    }

    std::size_t scanline_length()
    {
        return _scanline_length;
    }

    void clean_up()
    {
        ///@todo
    }

private:

    template< typename Src_View >
    void read_n_bits_row( byte_t* dst, int pos )
    {
        typedef rgb16_view_t dst_view_t;

        this->_io_dev.read_scanline( _buffer
                                   , pos
                                   , 0
                                   );

        if( this->_io_dev.are_bytes_swapped() )
        {
            _mirror_bites( _buffer );
        }

        Src_View src_view = interleaved_view( this->_info._width
                                            , 1
                                            , (typename Src_View::x_iterator) &_buffer.front()
                                            , scanline_length()
                                            );

        dst_view_t dst_view = interleaved_view( this->_info._width
                                              , 1
                                              , (dst_view_t::value_type*) dst
                                              , num_channels< dst_view_t >::value * 2 * this->_info._width
                                              );


        typename Src_View::x_iterator src_it = src_view.row_begin( 0 );
        dst_view_t::x_iterator dst_it        = dst_view.row_begin( 0 );

        for( dst_view_t::x_coord_t i = 0
           ; i < _info._width
           ; ++i, src_it++, dst_it++
           )
        {
            boost::uint16_t c = get_color( *src_it, gray_color_t() );
            *dst_it = this->_palette[c];
        }
    }

    void read_1_bit_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray1_image_t::view_t >( dst, pos );
    }

    void read_2_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray2_image_t::view_t >( dst, pos );
    }

    void read_4_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray4_image_t::view_t >( dst, pos );
    }

    void read_8_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray8_image_t::view_t >( dst, pos );
    }
    
    void read_16_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray16_image_t::view_t >( dst, pos );
    }

    void read_24_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray24_image_t::view_t >( dst, pos );
    }

    void read_32_bits_index_image( byte_t* dst, int pos )
    {
        read_n_bits_row< gray32_image_t::view_t >( dst, pos );
    }

    void read_row(byte_t* dst, int pos )
    {
         this->_io_dev.read_scanline( dst
                                    , pos
                                    , 0
                                    );

        if( this->_io_dev.are_bytes_swapped() )
        {
            _mirror_bites( dst, scanline_length() );
        }
    }

private:

    std::vector< byte_t > _buffer;

    detail::mirror_bits< std::vector< byte_t >, mpl::true_ > _mirror_bites;

    boost::function< void ( this_t*, byte_t*, int ) > _read_function;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP
