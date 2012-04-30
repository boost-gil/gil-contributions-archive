/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_IO_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_TIFF_IO_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/tiff_tags.hpp>

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

        read_header();
    }

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

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_IO_BACKEND_HPP
