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
   struct gray1_pixel_t {};
   struct rgb1_pixel_t {};
   struct rgba1_pixel_t {};
}

template< int BitsPerSample, int SampleFormat > struct channel_type_factory { typedef bits8 type; };
template<> struct channel_type_factory< 1, SAMPLEFORMAT_UINT > { typedef types::bits1  type; };
template<> struct channel_type_factory< 8, SAMPLEFORMAT_UINT > { typedef bits8  type; };
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

template< bool IsPlanar, typename Pixel > struct image_type_factory { typedef image< Pixel, IsPlanar > type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::gray1_pixel_t> { typedef bit_aligned_image1_type<1, gray_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgb1_pixel_t> { typedef bit_aligned_image1_type<1, rgb_layout_t>::type type; };
template< bool IsPlanar > struct image_type_factory<IsPlanar, types::rgba1_pixel_t> { typedef bit_aligned_image1_type<1, rgba_layout_t>::type type; };

template< bool IsPlanar > struct image_type_factory<IsPlanar, void> { typedef void type; };

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

template< typename View >
inline
void read_data( const View&    src_view
              , const point_t& top_left
              , unsigned int   plane
              , tiff_file_t    file     )
{
   typedef typename View::value_type pixel_t;

   std::size_t size_to_allocate = buffer_size< pixel_t >( src_view.width()
                                                        , file             );
   std::vector< pixel_t > buffer( size_to_allocate );

   // Skip over rows since for compressed images no random access is possible. See man
   // page ( diagnostics section ) for more information.
   for( uint32 row = 0; row < (uint32) top_left.y; ++row )
   {
      read_scaline( buffer
                  , row
                  , plane
                  , file   );
   }

   for( uint32 row = top_left.y; row < (uint32) src_view.height() + top_left.y; ++row )
   {
      read_scaline( buffer
                  , row
                  , plane
                  , file );

      std::copy( buffer.begin() + top_left.x
               , buffer.begin() + src_view.width() + top_left.x
               , src_view.row_begin( row - top_left.y            ));
   }
}



template< typename View >
inline
void read_bit_aligned_view( const View&    v
                          , const point_t& top_left 
                          , tiff_file_t    file     )
{
   typedef std::vector<unsigned char> buffer_t;

   tsize_t scanline_size_in_bytes = TIFFScanlineSize( file.get() );
   buffer_t buffer( scanline_size_in_bytes );

   unsigned char* first_byte = &buffer.front();
   unsigned char* last_byte  = &buffer.front() + ( scanline_size_in_bytes - 1 );

   View::x_iterator begin( first_byte, 0 );
   View::x_iterator end  ( last_byte , 7 );

   ///@todo: What about top_left?

   if( TIFFIsByteSwapped( file.get() ) > 0 )
   {
      // create a lookup table for swapping bits.
      buffer_t lookup( 256 );
      for( int i = 0; i < 256; ++i )
      {
         lookup[i] = swap_bits( i );
      }

      for( uint32 row = 0; row < (uint32) v.height(); ++row )
      {
         read_scaline( buffer
                     , row
                     , 0
                     , file   );

         
         buffer_t::iterator buffer_it  = buffer.begin();
         buffer_t::iterator buffer_end = buffer.end();

         for( ; buffer_it != buffer_end; ++buffer_it  )
         {
            *buffer_it = lookup[ *buffer_it ];
         }

         std::copy( begin, end, v.row_begin( row ) );
      }
   }
   else
   {
      // Don't swap bits.

      buffer_t::iterator buffer_it  = buffer.begin();
      buffer_t::iterator buffer_end = buffer.end();

      for( uint32 row = 0; row < (uint32) v.height(); ++row )
      {
         read_scaline( buffer
                     , row
                     , 0
                     , file   );

         std::copy( begin, end, v.row_begin( row ) );
      }
   }
}

template< typename Image_TIFF
        , typename View_User
        >
void read_interleaved_view_and_convert( const View_User& v
                                      , tiff_file_t      file
                                      , const point_t&   top_left
                                      , boost::mpl::true_         )
{
}

template< typename Image_TIFF
        , typename View_User
        , typename Color_Converter
        >
void read_interleaved_view_and_convert( const View_User& src_view
                                      , tiff_file_t      file
                                      , const point_t&   top_left
                                      , Color_Converter  cc
                                      , boost::mpl::false_        )
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
      std::transform( buffer.begin()
                    , buffer.begin() + src_view.width()
                    , src_view.row_begin( row )
                    , color_convert_deref_fn< typename View_TIFF::value_type
                                             , typename View_User::value_type
                                             , Color_Converter
                                             >( cc ));
   }
}

template< int Number_Of_Samples
        , typename Image_TIFF
        , typename View_User
        >
void read_planar_view_and_convert( const View_User& v
                                 , tiff_file_t      file
                                 , const point_t&   top_left
                                 , boost::mpl::true_          )
{}

template< int Number_Of_Samples
        , typename Image_TIFF
        , typename View_User
        , typename Color_Converter
        >
void read_planar_view_and_convert( const View_User& v
                                 , tiff_file_t      file
                                 , const point_t&   top_left
                                 , Color_Converter  cc
                                 , boost::mpl::false_        )
{
   typedef typename Image_TIFF::view_t View_TIFF;
   Image_TIFF tiff_img( v.dimensions() );

   typedef typename nth_channel_view_type<View_TIFF>::type plane_t;

   tsize_t scanline_size = TIFFScanlineSize( file.get() );
   std::vector< plane_t::value_type > buffer( scanline_size );

   for( tsample_t sample = 0
      ; sample < Number_Of_Samples
      ; ++sample                    )
   {
      for( uint32 row = 0; row < (uint32) v.height(); ++row )
      {
         read_scaline( buffer
                     , row
                     , sample
                     , file   );

         // copy into view
         std::copy( buffer.begin()
                  , buffer.begin() + v.width()
                  , nth_channel_view( view( tiff_img ), sample ).row_begin( row ));
      }
   }

   transform_pixels( view( tiff_img )
                   , v
                   , color_convert_deref_fn< typename View_TIFF::value_type
                                           , typename View_User::value_type
                                           , Color_Converter
                                           >( cc ));
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
