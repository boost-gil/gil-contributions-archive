/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_TIFF_IO_PRIVATE_HPP
#define GIL_TIFF_IO_PRIVATE_HPP

/// \file
/// \brief 
//
/// \author Christian Henning
/// 

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

#include <algorithm>
#include <string>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/gil/extension/io_new_2/tiff_tags.hpp>

#include <boost/gil/extension/io_new_2/detail/base.hpp>
#include <boost/gil/extension/io_new_2/detail/tiff_io_private.hpp>

namespace boost { namespace gil { namespace detail {

template < int K >
struct plane_recursion
{
   template< typename View
           , typename Device
           , typename ConversionPolicy
           >
   static
   void read_plane( const View& dst_view, reader< Device
                                                , tiff_tag
                                                , ConversionPolicy >* p )
   {
      typedef kth_channel_view_type< K, typename View >::type plane_t;
      plane_t plane = kth_channel_view<K>( dst_view );
      p->read_data( plane, K );

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
   void read_plane( const View& dst_view, reader< Device
                                                , tiff_tag
                                                , ConversionPolicy >* p ) {}
};


template< typename Device
        , typename ConversionPolicy
        >
struct reader< Device
             , tiff_tag
             , ConversionPolicy > 
{
public:

   reader( Device& device )
   : _io_dev( device )
   , _cc_policy()
   , _info()
   {}

   reader( Device&                                                device
         , typename const ConversionPolicy::color_converter_type& cc     )
   : _io_dev   ( device )
   , _cc_policy( cc     )
   , _info()
   {}

   image_read_info<tiff_tag> get_info()
   {
      image_read_info<tiff_tag> info;

      io_error_if( _io_dev.get_property<tiff_image_width>               ( info._width ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_image_height>              ( info._height ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_compression>               ( info._compression ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_samples_per_pixel>         ( info._samples_per_pixel ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_bits_per_sample>           ( info._bits_per_sample ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_sample_format>             ( info._sample_format ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_planar_configuration>      ( info._planar_configuration ) == false
                 , "cannot read tiff tag." );

      ///@todo: Doesn't work right now.
      //io_error_if( _io_dev.get_property<tiff_photometric_interpretation>( info._photometric_interpretation  ) == false
      //           , "cannot read tiff tag." );

      return info;
   }

    template< typename Image >
    void read_image( Image&         image
                   , const point_t& top_left )
    {
        _info     = get_info();
        _top_left = top_left;

        image.recreate( _info._width  - _top_left.x
                      , _info._height - _top_left.y );

        apply_impl( view( image ));
    }

    template<typename View>
    void read_view( View&          view
                  , const point_t& top_left )
    {
        _info     = get_info();
        _top_left = top_left;

        io_error_if( view.dimensions() !=  point_t( _info._width  - _top_left.x
                                                  , _info._height - _top_left.y )
                , "User provided view has incorrect size."                       );

        apply_impl( view );
    }

private:

   template< typename View >
   void apply_impl( View& dst_view )
   {
      if( _info._photometric_interpretation == PHOTOMETRIC_PALETTE )
      {
         // Steps:
         // 1. Read indices. It's an array of grayX_pixel_t.
         // 2. Read palette. It's an array of rgb16_pixel_t.
         // 3. ??? Create virtual image or transform the two arrays
         //    into a rgb16_image_t object. The latter might
         //    be a good first solution.

         unsigned int num_colors = 0;
         switch( _info._bits_per_sample )
         {
            case 1:  { read_palette_image< gray1_image_t  >( dst_view ); break; }
            case 2:  { read_palette_image< gray2_image_t  >( dst_view ); break; }
            case 4:  { read_palette_image< gray4_image_t  >( dst_view ); break; }
            case 8:  { read_palette_image< gray8_image_t  >( dst_view ); break; }
            case 16: { read_palette_image< gray16_image_t >( dst_view ); break; }

            default: { io_error( "Not supported palette " ); }
         }

         return;
      } //if

      // tiff image is not a palette image
      read_samples_per_pixel( dst_view );
   }

   template< typename PaletteImage
           , typename View
           >
   void read_palette_image( const View& dst_view )
   {
      PaletteImage indices( _info._width  - _top_left.x
                          , _info._height - _top_left.y );

      read_samples_per_pixel( view( indices ));

      read_palette_image( dst_view
                        , view( indices )
                        , boost::is_same< View, rgb16_view_t >::type() );
   }

   template< typename View
           , typename Indices_View
           >
   void read_palette_image( const View&         dst_view
                          , const Indices_View& indices_view
                          , boost::mpl::true_   // is View rgb16_view_t
                          )
   {
      tiff_color_map::red_t   red;
      tiff_color_map::green_t green;
      tiff_color_map::blue_t  blue;
   /*
      TIFFGetFieldDefaulted( file.get()
                           , tiff_color_map::tag
                           , &red
                           , &green
                           , &blue                 );

      typedef channel_traits< element_type< typename Indices_View::value_type >::type >::value_type channel_t;
      int num_colors = channel_traits< channel_t >::max_value();

      rgb16_planar_view_t palette = planar_rgb_view( num_colors
                                                   , 1
                                                   , red
                                                   , green
                                                   , blue
                                                   , sizeof( bits16 ) * num_colors );

      rgb16_planar_view_t::x_iterator palette_it = palette.row_begin( 0 );


      for( rgb16_view_t::y_coord_t y = 0; y < src_view.height(); ++y )
      {
         rgb16_view_t::x_iterator it  = src_view.row_begin( y );
         rgb16_view_t::x_iterator end = src_view.row_end( y );

         typename Indices_View::x_iterator indices_it = indices_view.row_begin( y );

         for( ; it != end; ++it, ++indices_it )
         {
            bits16 i = at_c<0>( *indices_it );

            *it = palette[i];
         }
      }
   */
   }

   template< typename View
           , typename Indices_View
           >
   inline
   void read_palette_image( const View&         dst_view
                          , const Indices_View& indices_view
                          , boost::mpl::false_  // is View rgb16_view_t
                          )
   {
      io_error( "User supplied image type must be rgb16_image_t." );
   }

   template< typename View >
   void read_samples_per_pixel( const View& dst_view )
   {
      switch( _info._samples_per_pixel )
      {
         case 1: { read_bits_per_sample< gray_layout_t >( dst_view ); break; }
         case 3: { read_bits_per_sample< rgb_layout_t  >( dst_view ); break; }
         case 4: { read_bits_per_sample< rgba_layout_t >( dst_view ); break; }

         default: { io_error( "Samples_Per_Pixel not supported." ); }
      }
   }

   template< typename Layout
           , typename View
           >
   void read_bits_per_sample( const View& dst_view )
   {
      switch( _info._bits_per_sample )
      {
         case 1:  { read_sample_format<  1, Layout >( dst_view ); break; }
         case 2:  { read_sample_format<  2, Layout >( dst_view ); break; }
         case 4:  { read_sample_format<  4, Layout >( dst_view ); break; }
         case 8:  { read_sample_format<  8, Layout >( dst_view ); break; }
         case 16: { read_sample_format< 16, Layout >( dst_view ); break; }
         case 32: { read_sample_format< 32, Layout >( dst_view ); break; }
         case 64: { read_sample_format< 64, Layout >( dst_view ); break; }

         default: { io_error( "Bits_Per_Sample not supported." ); }
      }
   }

   template< int      BitsPerSample
           , typename Layout
           , typename View
           >
   void read_sample_format( const View& dst_view )
   {
      switch( _info._sample_format )
      {
         case SAMPLEFORMAT_UINT:   { read_planar< BitsPerSample, SAMPLEFORMAT_UINT, Layout   >( dst_view ); break; }
         case SAMPLEFORMAT_INT:    { read_planar< BitsPerSample, SAMPLEFORMAT_INT, Layout    >( dst_view ); break; }
         case SAMPLEFORMAT_IEEEFP: { read_planar< BitsPerSample, SAMPLEFORMAT_IEEEFP, Layout >( dst_view ); break; }

         default: { io_error( "Sample format not supported." ); }
      }
   }

   template< int      BitsPerSample
           , int      SampleFormat
           , typename Layout
           , typename View
           >
   void read_planar( const View& dst_view )
   {
      typedef pixel_type_factory< BitsPerSample, SampleFormat, Layout >::type pixel_t;

      if( _info._planar_configuration == PLANARCONFIG_CONTIG )
      {
         typedef image_type_factory< pixel_t, false >::type image_t;

         typedef mpl::or_< is_same< pixel_t, not_allowed_t >::type
                         , is_same< image_t, not_allowed_t >::type
                         >::type unspecified_t;

         read_rows_interleaved< image_t >( dst_view
                                         , unspecified_t() );
      }
      else if( _info._planar_configuration == PLANARCONFIG_SEPARATE )
      {
         typedef image_type_factory< pixel_t, true >::type image_t;

         typedef mpl::or_< is_same< pixel_t, not_allowed_t >::type
                         , is_same< image_t, not_allowed_t >::type
                         >::type unspecified_t;

         read_rows_planar< image_t >( dst_view
                                    , unspecified_t() );
      }
      else
      {
         io_error( "Wrong planar configuration setting." );
      }
   }

   // specializations for unspecified image types.
   template< typename Tiff_Image, typename View > void read_rows_interleaved( View& dst_view, mpl::true_ ) { io_error( "Tiff image type isn't supported." ); }
   template< typename Tiff_Image, typename View > void read_rows_planar( View& dst_view, mpl::true_ ) { io_error( "Tiff image type isn't supported." ); }

   template< typename Tiff_Image
           , typename View
           >
   void read_rows_interleaved( View&       dst_view
                             , mpl::false_ // unspecified image type
                             )
   {
      typedef typename Tiff_Image::view_t      tiff_view_t;
      typedef typename tiff_view_t::value_type tiff_pixel_t;

      typedef typename View::value_type user_pixel_t;

      io_error_if( !ConversionPolicy::template is_allowed< tiff_pixel_t
                                                         , user_pixel_t 
                                                         >::type::value
                 , "User provided view has incorrect color space or channel type." );

      std::vector<tiff_pixel_t> buffer( dst_view.width() );

      skip_over_rows( buffer, 0 );

      for( std::ptrdiff_t row = _top_left.y
         ; row < dst_view.height() + _top_left.y
         ; ++row 
         )
      {
         _io_dev.read_scaline( buffer
                             , row
                             , 0      );

         _cc_policy.read( buffer.begin() + _top_left.x
                        , buffer.end()
                        , dst_view.row_begin( row )     );
      }
   }

   template< typename Tiff_Image
           , typename View
           >
   void read_rows_planar( View&       dst_view
                        , mpl::false_ // unspecified image type
                        )
   {
      typedef typename Tiff_Image::view_t      tiff_view_t;
      typedef typename tiff_view_t::value_type tiff_pixel_t;

      typedef typename View::value_type user_pixel_t;

      io_error_if( !ConversionPolicy::template is_allowed< tiff_pixel_t
                                                         , user_pixel_t 
                                                         >::type::value
                 , "User provided view has incorrect color space or channel type." );

      plane_recursion< num_channels< View >::value - 1 >::read_plane( dst_view
                                                                    , this      );
   }

   template< typename Buffer >
   void skip_over_rows( Buffer& buffer
                      , uint32  plane  )
   {
      if( _info._compression != COMPRESSION_NONE )
      {
         // Skipping over rows is not possible for compressed images(  no random access ). See man
         // page ( diagnostics section ) for more information.
         for( std::ptrdiff_t row = 0; row < _top_left.y; ++row )
         {
            _io_dev.read_scaline( buffer
                                , row
                                , plane  );
         }
      }
   }

   template< typename View >
   void read_data( const View& dst_view
                 , int         plane     )
   {
      typedef read_helper_for_compatible_views< is_bit_aligned< View >::type
                                              , View
                                              > read_helper_t;

      typedef read_helper_t::buffer_t buffer_t;

      std::size_t size_to_allocate = buffer_size< typename View::value_type >( dst_view.width()
                                                                             , is_bit_aligned< View >::type() );
      buffer_t buffer( size_to_allocate );
      read_helper_t::iterator_t begin = read_helper_t::begin( buffer );
      read_helper_t::iterator_t first = begin + _top_left.x;
      read_helper_t::iterator_t last  = first + dst_view.width();

      skip_over_rows( buffer, plane );

      swap_bits_fn< is_bit_aligned< View >::type, buffer_t > sb;

      for( std::ptrdiff_t row = _top_left.y
         ; row < dst_view.height() + _top_left.y
         ; ++row )
      {
         _io_dev.read_scaline( buffer
                             , row
                             , plane   );

         sb( buffer );

         _cc_policy.read( first
                        , last
                        , dst_view.row_begin( row ));
      }
   }

   template< typename is_bit_aligned
           , typename View
           >
   struct read_helper_for_compatible_views
   {
      typedef typename View::value_type element_t;
      typedef std::vector< element_t > buffer_t;
      typedef typename buffer_t::const_iterator iterator_t;

      static iterator_t begin( const buffer_t& buffer )
      {
         return iterator_t( buffer.begin() );
      }
                     
      static iterator_t end( const buffer_t& buffer )
      {
         return iterator_t( buffer.end() );
      }
   };

   template< typename View >
   struct read_helper_for_compatible_views< mpl::true_, View >
   {
      typedef unsigned char element_t;
      typedef std::vector< element_t > buffer_t;
      typedef typename bit_aligned_pixel_iterator< typename View::reference > iterator_t;

      static iterator_t begin( buffer_t& buffer )
      {
         return iterator_t( &buffer.front(), 0 );
      }
                     
      static iterator_t end( buffer_t& buffer )
      {
         // @todo Won't this break if 8 is not divisible by the bit depth? 
         // What if your bitdepth is 3. The last pixel may not have offset 0
         return iterator_t( &buffer.back() + 1, 0 );
      }
   };


   template< typename IsBitAligned
           , typename Buffer
           >
   struct swap_bits_fn
   {
      void operator() ( Buffer& ) {}
   };

   template<>
   struct swap_bits_fn< boost::mpl::true_, std::vector< unsigned char > >
   {
      swap_bits_fn()
      {
         for( int i = 0; i < 256; ++i )
         {
            _lookup[i] = swap_bits( i );
         }

         _swap_bits = ( _io_dev.are_bytes_swapped() > 0 ) ? true : false;
      }

      void operator() ( std::vector< unsigned char >& buffer )
      {
         typedef swap_bits_fn< boost::mpl::true_, std::vector< unsigned char > > tt;

         if( _swap_bits )
         {
            std::transform( buffer.begin()
                          , buffer.end()
                          , buffer.begin()
                          , boost::bind( &tt::_swap, *this, _1 ));
         }
      }
    
    private:
    
      unsigned char _swap( unsigned char byte ) const
      {
         return _lookup[ byte ];
      }
    
    private:
    
      boost::array< unsigned char, 256 > _lookup;
      bool _swap_bits;
   };

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
   std::size_t buffer_size( std::size_t width
                          , mpl::true_  // is_bit_aligned
                          )
   {
      return _io_dev.get_scanline_size();
   }

private:

   Device& _io_dev;

   ConversionPolicy _cc_policy;

   image_read_info<tiff_tag> _info;

   point_t _top_left;

   template < int K > friend struct plane_recursion;

};

template < typename Device >
struct writer< Device
             , tiff_tag
             > 
{
   writer( Device& file )
   : _out(file)
   {
   }

   ~writer()
   {
   }

   template<typename View>
   void apply( const View&    view
             , const point_t& top_left
             , boost::mpl::true_       )
   {
      write_rows( view
                , top_left );
   }

    template<typename View>
    void apply( const View& view
              , const point_t& top_left
              , boost::mpl::false_      )
    {
        write_rows( view, top_left);
    }

    template<typename View>
    void apply( const View&                       view
              , const point_t&                    top_left
              , const image_write_info<tiff_tag>& info      )
    {
        write_rows( view
                  , top_left );
    }

private:

    template< typename View >
    void write_rows( const View&    view
                   , const point_t& top_left )
    {
    }

    Device& _out;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // GIL_TIFF_IO_PRIVATE_HPP