/*
    Copyright 2007-2008 Christian Henning, Lubomir Bourdev
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
/// \date   2007-2008 \n
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

    , _red  ( NULL )
    , _green( NULL )
    , _blue ( NULL )
    {
        init_multipage_read( settings );
    }

    ~reader_backend()
    {
        _io_dev.set_close( true );
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

    // palette
    tiff_color_map::red_t   _red;
    tiff_color_map::green_t _green;
    tiff_color_map::blue_t  _blue;

    rgb16_planar_view_t _palette;
};

template< typename Device >
class scanline_reader< Device
                     , tiff_tag
                     >
    : public reader_backend< Device
                           , tiff_tag
                           >
{
public:

    scanline_reader( Device&                                device
                   , const image_read_settings< tiff_tag >& settings
                   )
    : reader_base< tiff_tag
                 >( device
                  , settings
                  )
    , 
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
            switch( this->_info._bits_per_sample )
            {
                _io_dev.get_field_defaulted( red, green, blue );

                case 1:
                {
                    typedef channel_type< get_pixel_type< gray1_image_t::view_t >::type >::type channel_t;

                    ///@todo should that be max_value + 1?
                    int num_colors = channel_traits< channel_t >::max_value();

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , red
                                              , green
                                              , blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_1_bit_image );

                    break;
                }

                case 2:
                {
                    typedef channel_type< get_pixel_type< gray2_image_t::view_t >::type >::type channel_t;

                    ///@todo should that be max_value + 1?
                    int num_colors = channel_traits< channel_t >::max_value();

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , red
                                              , green
                                              , blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_2_bits_image );

                    break;
                }
                case 4:
                {
                    typedef channel_type< get_pixel_type< gray4_image_t::view_t >::type >::type channel_t;

                    ///@todo should that be max_value + 1?
                    int num_colors = channel_traits< channel_t >::max_value();

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , red
                                              , green
                                              , blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_4_bits_image );

                    break;
                }

                case 8:  
                {
                    typedef channel_type< get_pixel_type< gray8_image_t::view_t >::type >::type channel_t;

                    ///@todo should that be max_value + 1?
                    int num_colors = channel_traits< channel_t >::max_value();

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , red
                                              , green
                                              , blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_8_bits_image );

                    break;
                }

                case 16:
                {
                    typedef channel_type< get_pixel_type< gray16_image_t::view_t >::type >::type channel_t;

                    ///@todo should that be max_value + 1?
                    int num_colors = channel_traits< channel_t >::max_value();

                    _palette = planar_rgb_view( num_colors
                                              , 1
                                              , red
                                              , green
                                              , blue
                                              , sizeof( bits16 ) * num_colors
                                              );

                    _read_function = boost::mem_fn( &this_t::read_16_bits_image );

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
                if( is_read_only::value == false )
                {
                    // the read_data function needs to know what gil type the source image is
                    // to have the default color converter function correctly

                    switch( this->_info._photometric_interpretation )
                    {
                        case PHOTOMETRIC_MINISWHITE:
                        case PHOTOMETRIC_MINISBLACK:
                        {
                            switch( this->_info._bits_per_sample )
                            {
                                case  1: { read_data< detail::row_buffer_helper_view< gray1_image_t::view_t > >( dst_view, 0 );  break; }
                                case  2: { read_data< detail::row_buffer_helper_view< gray2_image_t::view_t > >( dst_view, 0 );  break; }
                                case  4: { read_data< detail::row_buffer_helper_view< gray4_image_t::view_t > >( dst_view, 0 );  break; }
                                case  8: { read_data< detail::row_buffer_helper_view< gray8_view_t  > >( dst_view, 0 );  break; }
                                case 16: { read_data< detail::row_buffer_helper_view< gray16_view_t > >( dst_view, 0 );  break; }
                                case 32: { read_data< detail::row_buffer_helper_view< gray32_view_t > >( dst_view, 0 );  break; }
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
                                        case  8: { read_data< detail::row_buffer_helper_view< rgb8_view_t  > >( dst_view, 0 );  break; }
                                        case 16: { read_data< detail::row_buffer_helper_view< rgb16_view_t > >( dst_view, 0 );  break; }
                                        case 32: { read_data< detail::row_buffer_helper_view< rgb32_view_t > >( dst_view, 0 );  break; }
                                        default: { io_error( "Image type is not supported." ); }
                                    }

                                    break;
                                }

                                case 4:
                                {
                                    switch( this->_info._bits_per_sample )
                                    {
                                        case  8: { read_data< detail::row_buffer_helper_view< rgba8_view_t  > >( dst_view, 0 );  break; }
                                        case 16: { read_data< detail::row_buffer_helper_view< rgba16_view_t > >( dst_view, 0 );  break; }
                                        case 32: { read_data< detail::row_buffer_helper_view< rgba32_view_t > >( dst_view, 0 );  break; }
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
                                case  8: { read_data< detail::row_buffer_helper_view< cmyk8_view_t  > >( dst_view, 0 );  break; }
                                case 16: { read_data< detail::row_buffer_helper_view< cmyk16_view_t > >( dst_view, 0 );  break; }
                                case 32: { read_data< detail::row_buffer_helper_view< cmyk32_view_t > >( dst_view, 0 );  break; }
                            }

                            break;
                        }

                        default: { io_error( "Image type is not supported." ); }
                    }
                }
                else
                {
                    read_data< detail::row_buffer_helper_view< View > >( dst_view, 0 );
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
        _read_function(this, dst);
    }    

   std::size_t scanline_length()
   {
      return _io_dev.get_scanline_size();
   }

private:

   template< typename PaletteImage
           , typename View
           >
   void read_palette_image( const View& dst_view )
   {
      PaletteImage indices( this->_info._width  - this->_settings._top_left.x
                          , this->_info._height - this->_settings._top_left.y );

      // read the palette first
      read_data< detail::row_buffer_helper_view< typename PaletteImage::view_t > >( view( indices ), 0 );

      read_palette_image( dst_view
                        , view( indices )
                        , typename is_same< View, rgb16_view_t >::type() );
   }

   template< typename View
           , typename Indices_View
           >
   void read_palette_image( const View&         dst_view
                          , const Indices_View& indices_view
                          , mpl::true_   // is View rgb16_view_t
                          )
   {
      tiff_color_map::red_t   red   = NULL;
      tiff_color_map::green_t green = NULL;
      tiff_color_map::blue_t  blue  = NULL;

      _io_dev.get_field_defaulted( red, green, blue );

      typedef typename channel_traits<
                    typename element_type< 
                            typename Indices_View::value_type >::type >::value_type channel_t;

      int num_colors = channel_traits< channel_t >::max_value();

      rgb16_planar_view_t palette = planar_rgb_view( num_colors
                                                   , 1
                                                   , red
                                                   , green
                                                   , blue
                                                   , sizeof( bits16 ) * num_colors );

      typename rgb16_planar_view_t::x_iterator palette_it = palette.row_begin( 0 );

      for( typename rgb16_view_t::y_coord_t y = 0; y < dst_view.height(); ++y )
      {
         typename rgb16_view_t::x_iterator it  = dst_view.row_begin( y );
         typename rgb16_view_t::x_iterator end = dst_view.row_end( y );

         typename Indices_View::x_iterator indices_it = indices_view.row_begin( y );

         for( ; it != end; ++it, ++indices_it )
         {
            bits16 i = gil::at_c<0>( *indices_it );

            *it = palette[i];
         }
      }
   }

   template< typename Buffer >
   void skip_over_rows( Buffer& buffer
                      , int     plane
                      )
   {
      if( this->_info._compression != COMPRESSION_NONE )
      {
         // Skipping over rows is not possible for compressed images(  no random access ). See man
         // page ( diagnostics section ) for more information.
         for( std::ptrdiff_t row = 0; row < this->_settings._top_left.y; ++row )
         {
            _io_dev.read_scanline( buffer
                                 , row
                                 , static_cast< tsample_t >( plane ));
         }
      }
   }

   template< typename Buffer
           , typename View
           >
   void read_data( const View& dst_view
                 , int         plane     )
    {
        read_stripped_data< Buffer >( dst_view, 0 );
    }

   template< typename Buffer
           , typename View
           >
   void read_stripped_data( const View& dst_view
                          , int         plane     )
   {
      typedef typename is_bit_aligned< typename View::value_type >::type is_view_bit_aligned_t;

      //typedef row_buffer_helper_view< View > row_buffer_helper_t;
      typedef Buffer row_buffer_helper_t;

      typedef typename row_buffer_helper_t::buffer_t   buffer_t;
      typedef typename row_buffer_helper_t::iterator_t it_t;

      std::size_t size_to_allocate = buffer_size< typename View::value_type >( dst_view.width()
                                                                             , is_view_bit_aligned_t() );
      row_buffer_helper_t row_buffer_helper( size_to_allocate, true );

      it_t begin = row_buffer_helper.begin();

      it_t first = begin + this->_settings._top_left.x;
      it_t last  = first + this->_settings._dim.x; // one after last element

      // I don't think tiff allows for random access of row, that's why we need 
      // to read and discard rows when reading subimages.
      skip_over_rows( row_buffer_helper.buffer()
                    , plane
                    );

      detail::mirror_bits< buffer_t
                         , typename is_bit_aligned< View >::type
                         > mirror_bits( _io_dev.are_bytes_swapped() );

      std::ptrdiff_t row     = this->_settings._top_left.y;
      std::ptrdiff_t row_end = row + this->_settings._dim.y;
      std::ptrdiff_t dst_row = 0;

      for( 
         ; row < row_end
         ; ++row, ++dst_row
         )
      {
         _io_dev.read_scanline( row_buffer_helper.buffer()
                              , row
                              , static_cast< tsample_t >( plane )
                              );

         mirror_bits( row_buffer_helper.buffer() );

         this->_cc_policy.read( first
                              , last
                              , dst_view.row_begin( dst_row ));
      }
   }

   template< typename Pixel >
   std::size_t buffer_size( std::size_t width
                          , mpl::false_ // is_bit_aligned
                          )
   {
      std::size_t scanline_size_in_bytes = _io_dev.get_scanline_size();

      std::size_t element_size = sizeof( Pixel );

      return  std::max( width
                      , (( scanline_size_in_bytes + element_size - 1 ) / element_size ));
   }

   template< typename Pixel >
   std::size_t buffer_size( std::size_t /* width */
                          , mpl::true_  // is_bit_aligned
                          )
   {
      return _io_dev.get_scanline_size();
   }


private:

    boost::function< void ( this_t*, byte_t* ) > _read_function;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP
