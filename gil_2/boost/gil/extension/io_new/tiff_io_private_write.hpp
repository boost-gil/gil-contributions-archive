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

template <typename PixelReference>
struct my_interleaved_pixel_iterator_type_from_pixel_reference
{
private:
	typedef typename boost::remove_reference<PixelReference>::type::value_type pixel_value_t;
public:
	typedef typename iterator_type_from_pixel<pixel_value_t,false,false,true>::type type;
};


template< typename Channel
        , typename Layout
        , bool Mutable
        >
struct my_interleaved_pixel_iterator_type_from_pixel_reference< const bit_aligned_pixel_reference< uint8_t
                                                                                                 , Channel
                                                                                                 , Layout
                                                                                                 , Mutable
                                                                                                 > >
	: public iterator_type_from_pixel< const bit_aligned_pixel_reference< uint8_t
	                                                                    , Channel
	                                                                    , Layout
	                                                                    , Mutable
	                                                                    > 
	                                 ,false
	                                 ,false
	                                 ,true
	                                 >
{};

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
   void apply( const View&    src_view
             , const point_t& top_left )
   {
      typedef typename color_space_type<View>::type color_space_t;

      basic_tiff_image_write_info info;

      // write photometric interpretion - Warning: This value is rather subjective.
      // The user should better set this value itself. There is no way to decide if
      // a image is PHOTOMETRIC_MINISWHITE or PHOTOMETRIC_MINISBLACK. This writer
      // will assume PHOTOMETRIC_MINISBLACK for gray_t images and PHOTOMETRIC_RGB
      // for rgb_t images.
      info._photometric_interpretation = photometric_interpretation< color_space_t >::value;

      info._compression = COMPRESSION_LZW;
      info._orientation = ORIENTATION_TOPLEFT;

      info._planar_configuration = PLANARCONFIG_CONTIG;

      apply( src_view, top_left, info );
   }

   template< typename View >
   void apply( const View&                        src_view
             , const point_t&                     top_left
             , const basic_tiff_image_write_info& info     )
             
   {
      typedef typename View::value_type pixel_t;

      // get the type of the first channel (heterogeneous pixels might be broken for now!)
      typedef typename channel_traits<typename element_type< pixel_t >::type >::value_type channel_t;
      typedef my_interleaved_pixel_iterator_type_from_pixel_reference<typename View::reference>::type x_iterator;

      // write dimensions
      tiff_image_width::type width   = src_view.width();
      tiff_image_height::type height = src_view.height();

      set_property<tiff_image_width >( _file, width );
      set_property<tiff_image_height>( _file, height );

      // write planar configuration
      if( is_bit_aligned<View>::value == false )
      {
         set_property<tiff_planar_configuration>( _file, info._planar_configuration );
      }

      // write samples per pixel
      tiff_samples_per_pixel::type samples_per_pixel = num_channels< pixel_t >::value;
      set_property<tiff_samples_per_pixel>( _file, samples_per_pixel );

      // write bits per sample
      // @todo: Settings this value usually requires to write for each sample the bit
      // value seperately in case they are different, like rgb556.
      tiff_bits_per_sample::type bits_per_sample = unsigned_integral_num_bits<channel_t>::value;
      set_property<tiff_bits_per_sample>( _file, bits_per_sample );

      // write sample format
      tiff_sample_format::type sampl_format = sample_format< channel_t >::type::value;
      set_property<tiff_sample_format>( _file, sampl_format );

      // write photometric format
      set_property<tiff_photometric_interpretation>( _file, info._photometric_interpretation );

      // write compression
      set_property<tiff_compression>( _file, info._compression );

      // write orientation
      set_property<tiff_orientation>( _file, info._orientation );

      // write rows per strip
      tiff_rows_per_strip::type rows_per_strip = TIFFDefaultStripSize( _file.get(), 0 );
      set_property<tiff_rows_per_strip>( _file, rows_per_strip );

      // write the data
      std::size_t row_size_in_bytes = (src_view.width() * samples_per_pixel * bits_per_sample + 7) / 8;
      row_size_in_bytes -= ( top_left.x * samples_per_pixel * bits_per_sample + 7 ) / 8;

      std::vector< unsigned char > row( row_size_in_bytes );

      x_iterator row_it=x_iterator(&*row.begin());

      for( View::y_coord_t y = top_left.y; y < src_view.height(); ++y )
      {
         std::copy( src_view.row_begin( y ) + top_left.x
                  , src_view.row_end( y )
                  , row_it );

         write_scaline( row, y, 0, _file );

	      // @todo: do optional bit swapping here if you need to...
      }
   }

private:

   tiff_file_t _file;

};

} // namespace details
} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_WRITE_HPP
