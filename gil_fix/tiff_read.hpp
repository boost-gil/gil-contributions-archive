/*
    Copyright 2007-2008 Christian Henning, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

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


namespace boost { namespace gil { namespace detail {

template < int K >
struct plane_recursion
{
   template< typename View
           , typename Device
           , typename ConversionPolicy
           >
   static
   void read_plane( const View& dst_view
                  , reader< Device
                          , tiff_tag
                          , ConversionPolicy >* p
                  )
   {
      typedef typename kth_channel_view_type< K, View >::type plane_t;
      plane_t plane = kth_channel_view<K>( dst_view );
      if(!p->_io_dev.is_tiled())
        p->read_data( plane, K );
      else
          p->read_tiled_data( plane, K );

      plane_recursion< K - 1 >::read_plane( dst_view, p );
   }
};

template <>
struct plane_recursion< -1 >
{
   template< typename View
           , typename Device
           , typename ConversionPolicy
           >
   static
   void read_plane( const View&               /* dst_view */
                  , reader< Device
                          , tiff_tag
                          , ConversionPolicy
                          >*                  /* p         */
                  )
    {}
};

template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , tiff_tag
            , ConversionPolicy
            >
    : public reader_base< tiff_tag
                        , ConversionPolicy >
{
public:

    reader( Device&                                device
          , const image_read_settings< tiff_tag >& settings
          )
    : reader_base< tiff_tag
                 , ConversionPolicy
                 >( settings )
    , _io_dev( device )
    {
        init_multipage_read( settings );
    }

    reader( Device&                                                device
          , const typename ConversionPolicy::color_converter_type& cc
          , const image_read_settings< tiff_tag >&                 settings
          )
    : reader_base< tiff_tag
                 , ConversionPolicy
                 >( cc
                  , settings
                  )
    , _io_dev( device )
    {
        init_multipage_read( settings );
    }

   image_read_info<tiff_tag> get_info() const
   {
      image_read_info<tiff_tag> info;

      io_error_if( _io_dev.template get_property<tiff_image_width>               ( info._width ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_image_height>              ( info._height ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_compression>               ( info._compression ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_samples_per_pixel>         ( info._samples_per_pixel ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_bits_per_sample>           ( info._bits_per_sample ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_sample_format>             ( info._sample_format ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_planar_configuration>      ( info._planar_configuration ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.template get_property<tiff_photometric_interpretation>( info._photometric_interpretation  ) == false
                 , "cannot read tiff tag." );

      info._is_tiled = false;

      // Tile tags
      if( _io_dev.is_tiled() )
      {
          info._is_tiled = true;

          io_error_if( !_io_dev.template get_property< tiff_tile_width  >( info._tile_width )
                       , "cannot read tiff_tile_width tag." );
          io_error_if( !_io_dev.template get_property< tiff_tile_length >( info._tile_length )
                       , "cannot read tiff_tile_length tag." );
      }

      return info;
   }

    // only works for homogeneous image types
    template< typename View >
    void apply( View& dst_view )
    {
        if( this->_info._photometric_interpretation == PHOTOMETRIC_PALETTE )
        {
            // Steps:
            // 1. Read indices. It's an array of grayX_pixel_t.
            // 2. Read palette. It's an array of rgb16_pixel_t.
            // 3. ??? Create virtual image or transform the two arrays
            //    into a rgb16_image_t object. The latter might
            //    be a good first solution.

            switch( this->_info._bits_per_sample )
            {
                case 1:  { read_palette_image< gray1_image_t  >( dst_view ); break; }
                case 2:  { read_palette_image< gray2_image_t  >( dst_view ); break; }
                case 4:  { read_palette_image< gray4_image_t  >( dst_view ); break; }
                case 8:  { read_palette_image< gray8_image_t  >( dst_view ); break; }
                case 16: { read_palette_image< gray16_image_t >( dst_view ); break; }

                default: { io_error( "Not supported palette " ); }
            }

            return;

        }
        else
        {
            // In case we only read the image the user's type and
            // the tiff type need to compatible. Which means:
            // color_spaces_are_compatible && channels_are_pairwise_compatible

            typedef typename is_same< ConversionPolicy
                                    , read_and_no_convert
                                    >::type is_read_and_convert_t;

            io_error_if( !is_allowed< View >( this->_info
                                            , is_read_and_convert_t()
                                            )
                       , "Image types aren't compatible."
                       );

            if( this->_info._planar_configuration == PLANARCONFIG_SEPARATE )
            {
                plane_recursion< num_channels< View >::value - 1 >::read_plane( dst_view
                                                                              , this
                                                                              );
            }
            else if( this->_info._planar_configuration == PLANARCONFIG_CONTIG )
            {
                if( _io_dev.is_tiled() )
                {
                    read_tiled_data( dst_view, 0 );
                }
                else
                {
                    read_data( dst_view, 0 );
                }
            }
            else
            {
                io_error( "Wrong planar configuration setting." );
            }
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

   template< typename PaletteImage
           , typename View
           >
   void read_palette_image( const View& dst_view )
   {
      PaletteImage indices( this->_info._width  - this->_settings._top_left.x
                          , this->_info._height - this->_settings._top_left.y );


      if( _io_dev.is_tiled() )
      {
          read_tiled_data( view( indices ), 0 );
      }
      else
      {
          read_data( view( indices ), 0 );
      }

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

   template< typename View
           , typename Indices_View
           >
   inline
   void read_palette_image( const View&         /* dst_view     */
                          , const Indices_View& /* indices_view */
                          , mpl::false_  // is View rgb16_view_t
                          )
   {
      io_error( "User supplied image type must be rgb16_image_t." );
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

   template< typename View >
   void read_tiled_data( const View& dst_view
                       , int         plane
                       )
   {
       typedef row_buffer_helper_view< View >           row_buffer_helper_t;
       typedef typename row_buffer_helper_t::buffer_t   buffer_t;
       typedef typename row_buffer_helper_t::iterator_t it_t;

       // TIFFReadTile needs a buffer with enough memory allocated. This size can easily be computed by TIFFTileSize (implemented in device.hpp)
       row_buffer_helper_t row_buffer_helper( _io_dev.get_tile_size(), true );

       uint32_t plain_tile_size = this->_info._tile_width * this->_info._tile_length;

       mirror_bits< buffer_t
                  , typename is_bit_aligned< View >::type
                  > mirror_bits( _io_dev.are_bytes_swapped() );

       for( unsigned int y = 0; y < this->_info._height; y += this->_info._tile_length )
       {
           for( unsigned int x = 0; x < this->_info._width; x += this->_info._tile_width )
           {
               tiff_tile_width::type  tile_width  = this->_info._tile_width;
               tiff_tile_length::type tile_height = this->_info._tile_length;

               uint32_t current_tile_width  = (x + this->_info._tile_width <  this->_info._width  ) ? this->_info._tile_width  : this->_info._width  - x;
               uint32_t current_tile_length = (y + this->_info._tile_length < this->_info._height ) ? this->_info._tile_length : this->_info._height - y;

               _io_dev.read_tile( row_buffer_helper.buffer()
                                , x
                                , y
                                , 0
                                , static_cast< tsample_t >( plane )
                                );

               mirror_bits( row_buffer_helper.buffer() );

               point_t tile_top_left   ( x, y );
               point_t tile_lower_right( x + current_tile_width - 1, y + current_tile_length - 1 );

               point_t view_top_left   ( this->_settings._top_left );
               point_t view_lower_right( this->_settings._top_left.x + this->_settings._dim.x - 1
                                       , this->_settings._top_left.y + this->_settings._dim.y - 1 );

               if(  tile_top_left.x > view_lower_right.x
                 || tile_top_left.y > view_lower_right.y
                 || tile_lower_right.x < view_top_left.x
                 || tile_lower_right.y < view_top_left.y
                 )
               {
                   // current tile and dst_view do not overlap
                   continue;
               }
               else
               {
                   // dst_view is overlapping the current tile

                   typedef typename row_buffer_helper_t::view_t tile_view_t;

                   tile_view_t tile_view = row_buffer_helper.create_view( this->_info._tile_width
                                                                        , this->_info._tile_length
                                                                        );

                   // next is to define the portion in the tile that needs to be copied

                   std::ptrdiff_t img_x0 = ( tile_top_left.x >= view_top_left.x ) ? tile_top_left.x : view_top_left.x;
                   std::ptrdiff_t img_y0 = ( tile_top_left.y >= view_top_left.y ) ? tile_top_left.y : view_top_left.y;

                   std::ptrdiff_t img_x1 = ( tile_lower_right.x <= view_lower_right.x ) ? tile_lower_right.x : view_lower_right.x;
                   std::ptrdiff_t img_y1 = ( tile_lower_right.y <= view_lower_right.y ) ? tile_lower_right.y : view_lower_right.y;

                   std::ptrdiff_t tile_x0 = img_x0 - x;
                   std::ptrdiff_t tile_y0 = img_y0 - y;
                   std::ptrdiff_t tile_x1 = img_x1 - x;
                   std::ptrdiff_t tile_y1 = img_y1 - y;

                   assert( tile_x0 >= 0 && tile_y0 >= 0 && tile_x1 >= 0 && tile_y1 >= 0 );
                   assert( tile_x0 <= img_x1 && tile_y0 <= img_y1 );
                   assert( tile_x0 < tile_width && tile_y0 < tile_height && tile_x1 < tile_width && tile_y1 < tile_height );

                   std::ptrdiff_t tile_subimage_view_width  = tile_x1 - tile_x0 + 1;
                   std::ptrdiff_t tile_subimage_view_height = tile_y1 - tile_y0 + 1;

                   // copy (partial) tile rows into dst_view
                   tile_view_t tile_subimage_view = subimage_view( tile_view
                                                                 , (int) tile_x0
                                                                 , (int) tile_y0
                                                                 , (int) tile_subimage_view_width
                                                                 , (int) tile_subimage_view_height
                                                                 );

                   std::ptrdiff_t dst_x0 = img_x0 - this->_settings._top_left.x;
                   std::ptrdiff_t dst_y0 = img_y0 - this->_settings._top_left.y;

                   assert( dst_x0 >= 0 && dst_y0 >= 0 );

                   View dst_subimage_view = subimage_view( dst_view
                                                         , (int) dst_x0
                                                         , (int) dst_y0
                                                         , (int) tile_subimage_view_width
                                                         , (int) tile_subimage_view_height
                                                         );

                  // @todo wont work for conversions
                  copy_pixels( tile_subimage_view
                             , dst_subimage_view
                             );
               }
           } // for
       } // for
   }

   template< typename View >
   void read_data( const View& dst_view
                 , int         plane     )
   {
      typedef typename is_bit_aligned< typename View::value_type >::type is_view_bit_aligned_t;

      typedef row_buffer_helper_view< View > row_buffer_helper_t;

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

      mirror_bits< buffer_t
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

    bool point_in_rect( const point_t& rect_top_left
                      , const point_t& rect_lower_right
                      , const point_t& pt
                      )
    {
        return (  rect_top_left.x    <= pt.x
               && rect_top_left.y    <= pt.y
               && rect_lower_right.x >= pt.x
               && rect_lower_right.y >= pt.y );
    }

private:

   Device& _io_dev;

   template < int K > friend struct plane_recursion;
};

struct tiff_type_format_checker
{
    tiff_type_format_checker( const image_read_info< tiff_tag >& info )
    : _info( info )
    {}

    template< typename Image >
    bool apply()
    {
        typedef typename Image::view_t view_t;

        return is_allowed< view_t >( _info
                                   , mpl::true_()
                                   );
    }

private:

    const image_read_info< tiff_tag >& _info;
};

struct tiff_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , tiff_tag
                                           >
    {};
};

template< typename Device
        >
class dynamic_image_reader< Device
                          , tiff_tag
                          >
    : public reader< Device
                   , tiff_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , tiff_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                                device
                        , const image_read_settings< tiff_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        tiff_type_format_checker format_checker( this->_info );

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

            dynamic_io_fnobj< tiff_read_is_supported
                            , parent_t
                            > op( this );

            apply_operation( view( images )
                           , op
                           );
        }
    }
};


} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_IO_READ_HPP_INCLUDED
