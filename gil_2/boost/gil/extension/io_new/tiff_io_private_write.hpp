/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_IO_PRIVATE_WRITE_HPP
#define TIFF_IO_PRIVATE_WRITE_HPP

/// \file
/// \brief  
//
/// \author Christian Henning
///         

namespace boost { namespace gil { namespace detail {

typedef bit_aligned_image1_type< 1, gray_layout_t >::type gray1_image_t;
typedef gray1_image_t::view_t gray1_view_t;

template< typename View >
struct specific_writer_impl
{
   void operator() ( const View&                  v
                   , const tiff_image_write_info& info
                   , tiff_file_t                  file )
   {
      typedef typename View::value_type pixel_t;
      typedef channel_type< pixel_t >::type channel_t;
      typedef typename color_space_type<View>::type color_space_t;

      // write dimensions
      tiff_image_width::type width   = v.width();
      tiff_image_height::type height = v.height();

      set_property<tiff_image_width >( file, width, tiff_tag() );
      set_property<tiff_image_height>( file, height, tiff_tag() );

      // write planar configuration
      tiff_planar_configuration::type planar_config = PLANARCONFIG_CONTIG;
      set_property<tiff_planar_configuration>( file, planar_config, tiff_tag() );

      // write samples per pixel
      tiff_samples_per_pixel::type samples_per_pixel = num_channels< pixel_t >::value;
      set_property<tiff_samples_per_pixel>( file, samples_per_pixel, tiff_tag() );

      // write bits per sample
      tiff_bits_per_sample::type bits_per_sample = boost::mpl::at< bits_per_samples_map
                                                                  , channel_t
                                                                  >::type::value;
      set_property<tiff_bits_per_sample>( file, bits_per_sample, tiff_tag() );

      // write sample format
      tiff_sample_format::type sample_format = boost::mpl::at< sample_format_map
                                                             , channel_t 
                                                             >::type::value;

      set_property<tiff_sample_format>( file, sample_format, tiff_tag() );


      // write photometric format
      set_property<tiff_photometric_interpretation>( file, info._photometric_interpretation, tiff_tag() );

      // write compression
      set_property<tiff_compression>( file, info._compression, tiff_tag() );

      // write orientation
      set_property<tiff_orientation>( file, info._orientation, tiff_tag() );

      // write rows per strip
      tiff_rows_per_strip::type rows_per_strip = TIFFDefaultStripSize( file.get(), 0 );
      set_property<tiff_rows_per_strip>( file, rows_per_strip, tiff_tag() );

      // write the data
      std::vector< pixel_t > row( v.width() );

      for( View::y_coord_t y = 0; y < v.height(); ++y )
      {
         std::copy( v.row_begin( y ), v.row_end( y ), row.begin() );
         write_scaline( row, y, 0, file );
      }
   }
};

template<>
struct specific_writer_impl< gray1_view_t >
{
   typedef gray1_view_t::value_type gray1_pixel_t;
   typedef gray1_view_t::reference  gray1_reference_t;

   void operator() ( const gray1_view_t&          v
                   , const tiff_image_write_info& info
                   , tiff_file_t                  file )
   {
      // write dimensions
      tiff_image_width::type width   = v.width();
      tiff_image_height::type height = v.height();

      set_property<tiff_image_width >( file, width, tiff_tag() );
      set_property<tiff_image_height>( file, height, tiff_tag() );

      // write planar configuration
      tiff_planar_configuration::type planar_config = PLANARCONFIG_CONTIG;
      set_property<tiff_planar_configuration>( file, planar_config, tiff_tag() );

      // write samples per pixel
      tiff_samples_per_pixel::type samples_per_pixel = 1;
      set_property<tiff_samples_per_pixel>( file, samples_per_pixel, tiff_tag() );

      // write bits per sample
      tiff_bits_per_sample::type bits_per_sample = 1;
      set_property<tiff_bits_per_sample>( file, bits_per_sample, tiff_tag() );

      // write photometric format
      set_property<tiff_photometric_interpretation>( file, info._photometric_interpretation, tiff_tag() );

      // write compression
      set_property<tiff_compression>( file, info._compression, tiff_tag() );

      // write orientation
      set_property<tiff_orientation>( file, info._orientation, tiff_tag() );

      // write rows per strip
      tiff_rows_per_strip::type rows_per_strip = TIFFDefaultStripSize( file.get(), 0 );
      set_property<tiff_rows_per_strip>( file, rows_per_strip, tiff_tag() );

      std::vector< unsigned char > row( v.width() / 8 + 1 );

      for( gray1_view_t::y_coord_t y = 0; y < v.height(); ++y )
      {
         gray1_view_t::x_iterator it  = v.row_begin( y );
         gray1_view_t::x_iterator end = v.row_begin( y );

         unsigned char byte = 0;

         gray1_image_t::x_coord_t x = 0;
         for( ; x < v.width(); x += 8 )
         {
            _set_value( it[x]    , byte, 0x80 );
            _set_value( it[x + 1], byte, 0x40 );
            _set_value( it[x + 2], byte, 0x20 );
            _set_value( it[x + 3], byte, 0x10 );
            _set_value( it[x + 4], byte, 0x8  );
            _set_value( it[x + 5], byte, 0x4  );
            _set_value( it[x + 6], byte, 0x2  );
            _set_value( it[x + 7], byte, 0x1  );

            row[ x / 8 ] = byte;
         }

         {
            // take care of remainder pixels

            byte = 0;
            unsigned char or_value = 0x80;

            gray1_image_t::x_coord_t remainder = v.width() - ( x - 8 );

            for( int i = remainder; i > 0; --i )
            {
               _set_value( it[v.width() - i], byte, or_value );

               or_value = or_value >> 1;
            }

            row[ ( v.width() / 8 ) + 1 ] = byte;
         }

         write_scaline( row, y, 0, file );
      }
   }

private:

   void _set_value( const gray1_reference_t value
                  , unsigned char&          byte
                  , unsigned char           or_value )
   {
      if( value != gray1_pixel_t( 0 ) )
      {
         byte |= or_value;
      }
   }
};

template< typename View >
struct specific_writer
{
   void operator() ( const View&                  v
                   , const tiff_image_write_info& info
                   , tiff_file_t                  file )
   {
      specific_writer_impl<View> w;
      w( v, info, file );
   }
};


// Right now only gray_t and rgb_t images are supported. See comment below
// for the photometric interpretation.
class tiff_writer
{
public:

   tiff_writer( tiff_file_t file )
   : _file( file )
   {
   }

   template< typename View >
   void apply( const View& v )
   {
      typedef typename color_space_type<View>::type color_space_t;

      tiff_image_write_info info;

      // write photometric interpretion - Warning: This value is rather subjective.
      // The user should better set this value itself. There is no way to decide if
      // a image is PHOTOMETRIC_MINISWHITE or PHOTOMETRIC_MINISBLACK. This writer
      // will assume PHOTOMETRIC_MINISBLACK for gray_t images and PHOTOMETRIC_RGB
      // for rgb_t images.
      info._photometric_interpretation = boost::mpl::at< photometric_map
                                                       , color_space_t
                                                       >::type::value;

      info._compression = COMPRESSION_LZW;
      info._orientation = ORIENTATION_TOPLEFT;

      apply( v, info );
   }

   template< typename View >
   void apply( const View&                  v
             , const tiff_image_write_info& info )
   {
      specific_writer<View> writer;
      writer( v, info, _file );
   }

   template< typename View
           , typename Writer
           >
   void apply( const View&                  v
             , const tiff_image_write_info& info
             , Writer                       writer )
   {
      writer( v, info, _file );
   }

private:

   tiff_file_t _file;

};

} // namespace details
} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_WRITE_HPP
