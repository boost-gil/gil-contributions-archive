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

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

#include <boost/gil/utilities.hpp>

namespace boost { namespace gil { namespace detail {

typedef boost::mpl::map< boost::mpl::pair<boost::gil::bits8	 , boost::mpl::int_< 8  > >
                       , boost::mpl::pair<boost::gil::bits16 , boost::mpl::int_< 16 > >
                       , boost::mpl::pair<boost::gil::bits32 , boost::mpl::int_< 16 > >
                       , boost::mpl::pair<boost::gil::bits32f, boost::mpl::int_< 32 > >
                       , boost::mpl::pair<double             , boost::mpl::int_< 64 > >
                       , boost::mpl::pair<boost::gil::bits8s , boost::mpl::int_< 8  > >
                       , boost::mpl::pair<boost::gil::bits16s, boost::mpl::int_< 16 > >
                       , boost::mpl::pair<boost::gil::bits32s, boost::mpl::int_< 32 > >
                       > bits_per_samples_map;

typedef boost::mpl::map< boost::mpl::pair<boost::gil::bits8  , boost::mpl::int_< SAMPLEFORMAT_UINT   > >
                       , boost::mpl::pair<boost::gil::bits16 , boost::mpl::int_< SAMPLEFORMAT_UINT   > >
                       , boost::mpl::pair<boost::gil::bits32 , boost::mpl::int_< SAMPLEFORMAT_UINT   > >
                       , boost::mpl::pair<boost::gil::bits32f, boost::mpl::int_< SAMPLEFORMAT_IEEEFP > >
                       , boost::mpl::pair<double             , boost::mpl::int_< SAMPLEFORMAT_IEEEFP > >
                       , boost::mpl::pair<boost::gil::bits8s , boost::mpl::int_< SAMPLEFORMAT_INT    > >
                       , boost::mpl::pair<boost::gil::bits16s, boost::mpl::int_< SAMPLEFORMAT_INT    > >
                       , boost::mpl::pair<boost::gil::bits32s, boost::mpl::int_< SAMPLEFORMAT_INT    > >
                       > sample_format_map;


template< typename String >
tiff_file_t tiff_open_for_read( const String& file_name )
{
   BOOST_STATIC_ASSERT( false );
}

template<>
tiff_file_t tiff_open_for_read< std::string >( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "r" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

template< typename String >
tiff_file_t tiff_open_for_write( const String& file_name )
{
   BOOST_STATIC_ASSERT( false );
}

template<>
tiff_file_t tiff_open_for_write< std::string >( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
              , "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

template <typename Property>
bool get_property( tiff_file_t              file
                 , typename Property::type& value
                 , tiff_tag                        )
{
   if( TIFFGetFieldDefaulted( file.get(), Property::tag, &value ) == 1 )
   {
      return true;
   }

   return false;
}

template <typename Property>
bool set_property( tiff_file_t              file
                 , typename Property::type& value
                 , tiff_tag                        )
{
   if( TIFFSetField( file.get(), Property::tag, &value ) == 1 )
   {
      return true;
   }

   return false;
}

template< typename View >
inline
void read_interleaved_view( const View&    v
                          , tiff_file_t    file
                          , const point_t& top_left 
                          , boost::mpl::true_       )
{
}

template< typename View >
inline
void read_interleaved_view( const View     v
                          , tiff_file_t    file
                          , const point_t& top_left 
                          , boost::mpl::false_ )
{
   tsize_t scanline_size = TIFFScanlineSize( file.get() );

   std::size_t element_size = sizeof( View::value_type );

   std::size_t dd = ((std::size_t) scanline_size + element_size - 1 ) / element_size;

   std::size_t size_to_allocate = std::max( (std::size_t) v.width()
                                          , dd        );

   std::vector< View::value_type > buffer( size_to_allocate );

   View vv = interleaved_view( v.width()
                              , 1
                              , static_cast<View::x_iterator>( &buffer.front() )
                              , size_to_allocate * element_size                         );

   for( uint32 row = 0; row < (uint32) v.height(); ++row )
   {
      read_scaline( buffer
                  , row
                  , 0
                  , file );

      // copy into view
      copy_pixels( vv
                  , subimage_view( v
                                 , point_t( 0
                                          , row )
                                 , point_t( v.width()
                                          , 1          )));
   }
}

template< int Number_Of_Samples
        , typename View
        >
inline
void read_planar_view( const View& v
                     , tiff_file_t file
                     , const point_t& top_left 
                     , boost::mpl::true_       )
{
}

template< int Number_Of_Samples
        , typename View
        >
inline
void read_planar_view( const View&    v
                     , tiff_file_t    file
                     , const point_t& top_left 
                     , boost::mpl::false_      )
{
   typedef nth_channel_view_type<View>::type plane_t;

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
                  , nth_channel_view( v, sample ).row_begin( row ));
      }
   }
}


template< typename View >
inline
void read_bit_aligned_view( const View&    v
                          , tiff_file_t    file
                          , const point_t& top_left 
                          , boost::mpl::false_      )
{
}

template< typename View >
inline
void read_bit_aligned_view( const View&    v
                          , tiff_file_t    file
                          , const point_t& top_left 
                          , boost::mpl::true_       )
{
   tsize_t scanline_size_in_bytes = TIFFScanlineSize( file.get() );
   std::vector<unsigned char> buffer( scanline_size_in_bytes );

   unsigned char* first_byte = &buffer.front();
   unsigned char* last_byte  = &buffer.front() + ( scanline_size_in_bytes - 1 );

   View::x_iterator begin( first_byte, 0 );
   View::x_iterator end  ( last_byte , 7 );

   for( uint32 row = 0; row < (uint32) v.height(); ++row )
   {
      read_scaline( buffer
                  , row
                  , 0
                  , file   );

      std::transform( buffer.begin(), buffer.end(), buffer.begin(), swap_bits );

      std::copy( begin, end, v.row_begin( row ) );
   }

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

template< typename Buffer >
inline 
void read_strip( Buffer&     buffer
               , tstrip_t    number_of_strip
               , tsize_t     strip_size
               , tiff_file_t file             )
{
   int size = TIFFReadEncodedStrip( file.get()
                                    , number_of_strip
                                    , &buffer.front()
                                    , strip_size      );

   io_error_if( size == -1, "Read error." );
}



} // namespace details
} // namespace gil
} // namespace boost

#endif // TIFF_BASE_HPP
