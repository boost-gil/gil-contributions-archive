/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_BASE_HPP
#define TIFF_BASE_HPP

/// \file
/// \brief  Defining some basic libtiff wrappers.
//
/// \author Christian Henning
///         

#include <boost/bind.hpp>

#include <boost/gil/utilities.hpp>

namespace boost { namespace gil { namespace detail {

template < typename Channel > struct sample_format : public mpl::int_<SAMPLEFORMAT_UINT> {};
template<> struct sample_format<bits8>   : public mpl::int_<SAMPLEFORMAT_UINT> {};
template<> struct sample_format<bits16>  : public mpl::int_<SAMPLEFORMAT_UINT> {};
template<> struct sample_format<bits32>  : public mpl::int_<SAMPLEFORMAT_UINT> {};
template<> struct sample_format<bits32f> : public mpl::int_<SAMPLEFORMAT_IEEEFP> {};
template<> struct sample_format<double>  : public mpl::int_<SAMPLEFORMAT_IEEEFP> {};
template<> struct sample_format<bits8s>  : public mpl::int_<SAMPLEFORMAT_INT> {};
template<> struct sample_format<bits16s> : public mpl::int_<SAMPLEFORMAT_INT> {};
template<> struct sample_format<bits32s> : public mpl::int_<SAMPLEFORMAT_INT> {};

template <typename Channel> struct photometric_interpretation {};
template<> struct photometric_interpretation< gray_t > : public boost::mpl::int_< PHOTOMETRIC_MINISBLACK > {};
template<> struct photometric_interpretation< rgb_t  > : public boost::mpl::int_< PHOTOMETRIC_RGB        > {};
template<> struct photometric_interpretation< rgba_t > : public boost::mpl::int_< PHOTOMETRIC_RGB        > {};
template<> struct photometric_interpretation< cmyk_t > : public boost::mpl::int_< PHOTOMETRIC_SEPARATED  > {};

namespace types
{
   struct bits1 {};
   struct bits2 {};
   struct bits4 {};

   struct gray1_pixel_t {};
   struct rgb1_pixel_t {};
   struct rgba1_pixel_t {};

   struct gray2_pixel_t {};
   struct rgb2_pixel_t {};
   struct rgba2_pixel_t {};

   struct gray4_pixel_t {};
   struct rgb4_pixel_t {};
   struct rgba4_pixel_t {};
}

template< int BitsPerSample, int SampleFormat > struct channel_type_factory { typedef bits8 type; };
template<> struct channel_type_factory< 1, SAMPLEFORMAT_UINT > { typedef types::bits1  type; };
template<> struct channel_type_factory< 2, SAMPLEFORMAT_UINT > { typedef types::bits2  type; };
template<> struct channel_type_factory< 4, SAMPLEFORMAT_UINT > { typedef types::bits4  type; };
template<> struct channel_type_factory< 8, SAMPLEFORMAT_INT >  { typedef bits8s type; };
template<> struct channel_type_factory< 16, SAMPLEFORMAT_UINT > { typedef bits16  type; };
template<> struct channel_type_factory< 16, SAMPLEFORMAT_INT >  { typedef bits16s type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_UINT   > { typedef bits32  type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_INT    >  { typedef bits32s type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_IEEEFP >  { typedef bits32f type; };
template<> struct channel_type_factory< 64, SAMPLEFORMAT_IEEEFP >  { typedef double type; };


template< typename Channel, typename Layout > struct pixel_type_factory { typedef void type; };
template< typename Channel > struct pixel_type_factory< Channel, gray_layout_t > { typedef pixel< Channel, gray_layout_t > type; };
template< typename Channel > struct pixel_type_factory< Channel, rgb_layout_t > { typedef pixel< Channel, rgb_layout_t > type; };
template< typename Channel > struct pixel_type_factory< Channel, rgba_layout_t > { typedef pixel< Channel, rgba_layout_t > type; };
template<> struct pixel_type_factory< types::bits1, gray_layout_t > { typedef types::gray1_pixel_t type; };
template<> struct pixel_type_factory< types::bits1, rgb_layout_t  > { typedef types::rgb1_pixel_t  type; };
template<> struct pixel_type_factory< types::bits1, rgba_layout_t > { typedef types::rgba1_pixel_t type; };

template<> struct pixel_type_factory< types::bits2, gray_layout_t > { typedef types::gray2_pixel_t type; };
template<> struct pixel_type_factory< types::bits2, rgb_layout_t  > { typedef types::rgb2_pixel_t  type; };
template<> struct pixel_type_factory< types::bits2, rgba_layout_t > { typedef types::rgba2_pixel_t type; };

template<> struct pixel_type_factory< types::bits4, gray_layout_t > { typedef types::gray4_pixel_t type; };
template<> struct pixel_type_factory< types::bits4, rgb_layout_t  > { typedef types::rgb4_pixel_t  type; };
template<> struct pixel_type_factory< types::bits4, rgba_layout_t > { typedef types::rgba4_pixel_t type; };

template< bool IsPlanar, typename Pixel > struct image_type_factory { typedef image< Pixel, IsPlanar > type; };

// Specializations for bit_aligned images.
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::gray1_pixel_t> { typedef bit_aligned_image1_type<1, gray_layout_t>::type type; };

template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgb1_pixel_t> { typedef bit_aligned_image3_type<1,1,1, rgb_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgba1_pixel_t> { typedef bit_aligned_image4_type<1,1,1,1, rgba_layout_t>::type type; };

template< bool IsPlanar > struct image_type_factory<IsPlanar, types::gray2_pixel_t> { typedef bit_aligned_image1_type<2, gray_layout_t>::type type; };

template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgb2_pixel_t> { typedef bit_aligned_image3_type<2,2,2, rgb_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgba2_pixel_t> { typedef bit_aligned_image4_type<2,2,2,2, rgba_layout_t>::type type; };

template< bool IsPlanar > struct image_type_factory<IsPlanar, types::gray4_pixel_t> { typedef bit_aligned_image1_type<4, gray_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgb4_pixel_t> { typedef bit_aligned_image3_type<4,4,4, rgb_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgba4_pixel_t> { typedef bit_aligned_image4_type<4,4,4,4, rgba_layout_t>::type type; };

// Total specializations for bit_aligned images that are not allowed.
template< bool IsPlanar > struct image_type_factory<IsPlanar, void> { typedef void type; };
template<> struct image_type_factory<true, types::gray1_pixel_t> { typedef void type; };
template<> struct image_type_factory<true, types::gray2_pixel_t> { typedef void type; };
template<> struct image_type_factory<true, types::gray4_pixel_t> { typedef void type; };
template<> struct image_type_factory< true, gray8_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray16_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray32_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray8s_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray16s_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray32s_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, gray32f_pixel_t > { typedef void type; };
template<> struct image_type_factory< true, pixel_type_factory< double, gray_layout_t >::type > { typedef void type; };


tiff_file_t tiff_open_for_read( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "r" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

tiff_file_t tiff_open_for_read( const std::wstring& file_name )
{
   return tiff_open_for_read( convert_to_string( file_name ));
}

tiff_file_t tiff_open_for_write( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

tiff_file_t tiff_open_for_write( const std::wstring& file_name )
{
   return tiff_open_for_write( convert_to_string( file_name ));
}


template <typename Property>
bool get_property( tiff_file_t              file
                 , typename Property::type& value )
{
   if( TIFFGetFieldDefaulted( file.get(), Property::tag, &value ) == 1 )
   {
      return true;
   }

   return false;
}

template <typename Property>
bool set_property( tiff_file_t                    file
                 , const typename Property::type& value )
{
   if( TIFFSetField( file.get(), Property::tag, value ) == 1 )
   {
      return true;
   }

   return false;
}

template< typename Pixel >
inline
std::size_t buffer_size( std::size_t width
                       , tiff_file_t file   )
{
   std::size_t scanline_size_in_bytes = TIFFScanlineSize( file.get() );

   std::size_t element_size = sizeof( Pixel );

   return  std::max( width
                   , (( scanline_size_in_bytes + element_size - 1 ) / element_size ));
}

template< typename Buffer >
inline
void skip_over_rows( uint32                            end
                   , uint32                            plane
                   , Buffer&                           buffer
                   , const basic_tiff_image_read_info& info
                   , tiff_file_t                       file   )
{
   if( info._compression != COMPRESSION_NONE )
   {
      // Skip over rows since for compressed images no random access is possible. See man
      // page ( diagnostics section ) for more information.
      for( uint32 row = 0; row < end; ++row )
      {
         read_scaline( buffer
                     , row
                     , plane
                     , file   );
      }
   }
}

template< typename is_bit_aligned
        , typename Buffer
        >
struct swap_bits_fn
{
   swap_bits_fn( tiff_file_t file ) {}

   void operator() ( Buffer& ) {}
};

template<>
struct swap_bits_fn< boost::mpl::true_, std::vector< unsigned char > >
{
   swap_bits_fn( tiff_file_t file ) : _lookup( 256 )
   {
      for( int i = 0; i < 256; ++i )
      {
         _lookup[i] = swap_bits( i );
      }

      _swap_bits = ( TIFFIsByteSwapped( file.get() ) > 0 ) ? true : false;
   }

   void operator() ( std::vector< unsigned char >& buffer )
   {
      typedef swap_bits_fn< boost::mpl::true_, std::vector< unsigned char > > tt;

      if( _swap_bits )
      {
         std::transform( buffer.begin(), buffer.end(), buffer.begin(), boost::bind( &tt::_swap, *this, _1 ));
      }
   }
 
 private:
 
   unsigned char _swap( unsigned char byte )
   {
      return _lookup[ byte ];
   }
 
 private:
 
   std::vector< unsigned char > _lookup;
   bool _swap_bits;
};



template< typename View >
inline
void read_data( const View&                       src_view
              , const point_t&                    top_left
              , unsigned int                      plane
              , const basic_tiff_image_read_info& info
              , tiff_file_t                       file     )
{

   typedef read_helper_for_compatible_views< is_bit_aligned< View >::type
                                           , View
                                           > read_helper_t;

   typedef read_helper_t::buffer_t buffer_t;

   std::size_t size_to_allocate = buffer_size< View::value_type >( src_view.width()
                                                              , file             );

   buffer_t buffer( size_to_allocate );
   read_helper_t::iterator_t begin = read_helper_t::begin( buffer );
   read_helper_t::iterator_t first = begin + top_left.x; 
   read_helper_t::iterator_t last  = begin + src_view.width() + top_left.x;

   skip_over_rows( (uint32) top_left.y, plane, buffer, info, file );

   swap_bits_fn< is_bit_aligned< View >::type, buffer_t > sb( file );

   for( uint32 row = top_left.y; row < (uint32) src_view.height() + top_left.y; ++row )
   {
      read_scaline( buffer, row, plane, file );

      sb( buffer );

      std::copy( first, last, src_view.row_begin( row - top_left.y ));
   }
}

template< typename Image_TIFF
        , typename View_User
        , typename Color_Converter
        >
void read_interleaved_data_and_convert( const View_User&                  src_view
                                      , const point_t&                    top_left
                                      , Color_Converter                   cc
                                      , const basic_tiff_image_read_info& info
                                      , tiff_file_t                       file        )
{
   typedef typename Image_TIFF::view_t View_TIFF;

   std::size_t size_to_allocate = buffer_size<View_TIFF::value_type>( src_view.width()
                                                                    , file             );

   std::vector< View_TIFF::value_type > buffer( size_to_allocate );

   for( uint32 row = 0; row < (uint32) src_view.height(); ++row )
   {
      read_scaline( buffer
                  , row
                  , 0
                  , file );

      // convert data
//@ todo: doesn't compile
/*
      std::transform( buffer.begin() + top_left.x
                    , buffer.begin() + src_view.width() + top_left.x
                    , src_view.row_begin( row - top_left.y )
                    , color_convert_deref_fn< typename View_TIFF::value_type
                                            , typename View_User::value_type
                                            , Color_Converter
                                            >( cc ));
*/
   }
}

template< typename View
        , typename Indices_View
        >
void read_palette_image( const View&                       src_view
                       , const Indices_View&               indices_view
                       , const basic_tiff_image_read_info& info
                       , tiff_file_t                       file
                       , boost::mpl::true_                                )
{
   tiff_color_map::red   red;
   tiff_color_map::green green;
   tiff_color_map::blue  blue;

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
         unsigned char i = at_c<0>( *indices_it );

         *it = *palette.xy_at( i, 0 );
      }
   }
}

template< typename View
        , typename Indices_View
        >
void read_palette_image( const View&                       src_view
                       , const Indices_View&               indices_view
                       , const basic_tiff_image_read_info& info
                       , tiff_file_t                       file
                       , boost::mpl::false_                                 ) {}


template< typename Buffer >
inline 
void read_scaline( Buffer&     buffer
                 , uint32      row
                 , tsample_t   plane
                 , tiff_file_t file             )
{
   int size = TIFFReadScanline( file.get()
                              , &buffer.front()
                              , row
                              , plane           );

   io_error_if( size == -1, "Read error." );
}

template< typename Buffer >
inline 
void write_scaline( Buffer&     buffer
                  , uint32      row
                  , tsample_t   plane
                  , tiff_file_t file             )
{
   int size = TIFFWriteScanline( file.get()
                               , &buffer.front()
                               , row
                               , plane           );

   io_error_if( size == -1, "Write error." );
}

} // namespace details
} // namespace gil
} // namespace boost

#endif // TIFF_BASE_HPP
