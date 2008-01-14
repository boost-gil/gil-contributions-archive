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

#include <boost/array.hpp>
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


typedef boost::shared_ptr<TIFF> tiff_file_t;

inline
tiff_file_t tiff_open_for_read( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "r" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

inline
tiff_file_t tiff_open_for_read( const std::wstring& file_name )
{
   return tiff_open_for_read( convert_to_string( file_name ));
}

inline
tiff_file_t tiff_open_for_write( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

inline
tiff_file_t tiff_open_for_write( const std::wstring& file_name )
{
   return tiff_open_for_write( convert_to_string( file_name ));
}


template <typename Property>
inline
bool get_property( const tiff_file_t        file
                 , typename Property::type& value )
{
   if( TIFFGetFieldDefaulted( file.get(), Property::tag, &value ) == 1 )
   {
      return true;
   }

   return false;
}

template <typename Property>
inline
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
                       , tiff_file_t file
                       , mpl::false_       )
{
   std::size_t scanline_size_in_bytes = TIFFScanlineSize( file.get() );

   std::size_t element_size = sizeof( Pixel );

   return  std::max( width
                   , (( scanline_size_in_bytes + element_size - 1 ) / element_size ));
}

template< typename Pixel >
inline
std::size_t buffer_size( std::size_t width
                       , tiff_file_t file
                       , mpl::true_         )
{
   return TIFFScanlineSize( file.get() );
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

template< typename IsBitAligned
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
   swap_bits_fn( tiff_file_t file )
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

   std::size_t size_to_allocate = buffer_size< typename View::value_type >( src_view.width()
                                                                          , file  
                                                                          , is_bit_aligned< View >::type() );

   buffer_t buffer( size_to_allocate );
   read_helper_t::iterator_t begin = read_helper_t::begin( buffer );
   read_helper_t::iterator_t first = begin + top_left.x; 
   read_helper_t::iterator_t last  = first + src_view.width();

   skip_over_rows( (uint32) top_left.y, plane, buffer, info, file );

   swap_bits_fn< is_bit_aligned< View >::type, buffer_t > sb( file );

   for( uint32 row = (uint32) top_left.y; row < (uint32) src_view.height() + (uint32) top_left.y; ++row )
   {
      read_scaline( buffer, row, plane, file );

      sb( buffer );

      std::copy( first, last, src_view.row_begin( row - top_left.y ));
   }
}

template< typename IsBitAligned
        , typename ColorSpace
        , typename View
        >
struct converter_workaround
{
   typedef read_helper_for_compatible_views< IsBitAligned
                                           , View
                                           > read_helper_t;

   template< typename Dst_View
           , typename Color_Converter
           >
   void operator() ( typename read_helper_t::iterator_t first
                   , typename read_helper_t::iterator_t last
                   , const Dst_View&                    dst_view
                   , const point_t&                     top_left
                   , unsigned int                       row
                   , Color_Converter&                   cc       )
   {
      std::transform( first
                    , last
                    , dst_view.row_begin( row - top_left.y )
                    , color_convert_deref_fn< typename View::value_type
                                            , typename Dst_View::value_type
                                            , Color_Converter
                                            >( cc ));
   }
};

template< typename View >
struct converter_workaround< boost::mpl::true_, rgba_t, View >
{
   typedef read_helper_for_compatible_views< boost::mpl::true_
                                           , View
                                           > read_helper_t;

   template< typename Dst_View
           , typename Color_Converter
           >
   void operator() ( typename read_helper_t::iterator_t first
                   , typename read_helper_t::iterator_t last
                   , const Dst_View&           dst_view
                   , const point_t&            top_left
                   , unsigned int              row
                   , Color_Converter&          cc       )
   {
      io_error( "Not implemented yet." );
   }
};

template< typename Image_TIFF
        , typename View_User
        , typename Color_Converter
        >
inline
void read_interleaved_data_and_convert( const View_User&                  src_view
                                      , const point_t&                    top_left
                                      , Color_Converter                   cc
                                      , const basic_tiff_image_read_info& info
                                      , tiff_file_t                       file        )
{
   // @todo gil convention: end typedefs with _t unless they already exist in say STL
   typedef typename Image_TIFF::view_t View_TIFF;


   typedef read_helper_for_compatible_views< is_bit_aligned< View_TIFF >::type
                                           , View_TIFF
                                           > read_helper_t;

   typedef read_helper_t::buffer_t buffer_t;

   std::size_t size_to_allocate = buffer_size< typename View_TIFF::value_type >( src_view.width()
                                                                               , file    
                                                                               , is_bit_aligned< View >::type() );

   buffer_t buffer( size_to_allocate );
   read_helper_t::iterator_t begin = read_helper_t::begin( buffer );
   read_helper_t::iterator_t first = begin + top_left.x; 
   read_helper_t::iterator_t last  = begin + src_view.width() + top_left.x;

   skip_over_rows( (uint32) top_left.y, 0, buffer, info, file );

   swap_bits_fn< is_bit_aligned< View_TIFF >::type, buffer_t > sb( file );

   // @todo: Default color converter doesn't work for bit aligned rgba image types when being
   // the source. See color_convert.hpp[270]. The metafunction channel_type does work
   // for bit aligned images.
   converter_workaround< typename is_bit_aligned< View_TIFF >::type
                       , typename color_space_type< typename View_TIFF::value_type >::type
                       , View_TIFF
                       > cw;

   for( uint32 row = top_left.y; row < (uint32) src_view.height() + top_left.y; ++row )
   {
      read_scaline( buffer, row, 0, file );

      sb( buffer );

      cw( first, last, src_view, top_left, row, cc );
   }
}

template< typename View
        , typename Indices_View
        >
inline
void read_palette_image( const View&                       src_view
                       , const Indices_View&               indices_view
                       , const basic_tiff_image_read_info& info
                       , tiff_file_t                       file
                       , boost::mpl::true_                                )
{
   tiff_color_map::red_t   red;
   tiff_color_map::green_t green;
   tiff_color_map::blue_t  blue;

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
}

template< typename View
        , typename Indices_View
        >
inline
void read_palette_image( const View&                       src_view
                       , const Indices_View&               indices_view
                       , const basic_tiff_image_read_info& info
                       , tiff_file_t                       file
                       , boost::mpl::false_          /* is View rgb16_view_t*/ )
{
   io_error( "User supplied image type must be rgb16_image_t." );
}


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
