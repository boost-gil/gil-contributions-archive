// io_new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "boost/gil/extension/io_new/tiff_io.hpp"

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

using namespace std;
using namespace boost;
using namespace gil;
using namespace fusion;


static void tiff_error_handler(const char *s1, const char *s2, va_list ap)
{
   std::stringstream ss;
   ss << "ERROR: " << s1 << ": ";

   std::vector<char> buffer( 100 );
   vsprintf( &buffer.front(), s2, ap );

   ss.write( &buffer.front(), 100 );
   cout << ss.str() << endl;
}

static void tiff_warning_handler(const char *s1, const char *s2, va_list ap)
{
   stringstream ss;
   ss << "WARNING: " << s1 << ": ";

   std::vector<char> buffer( 100 );
   vsprintf( &buffer.front(), s2, ap );

   ss.write( &buffer.front(), 100 );
   cout << ss.str() << endl;
}

typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
typedef image< rgb64f_pixel_t, true > rgb64f_planar_image_t;
typedef rgb64f_planar_image_t::view_t rgb64f_planar_view_t;

struct my_color_converter {

   my_color_converter( const rgb64f_pixel_t& min
                     , const rgb64f_pixel_t& max ) 
   : _red_range  ( get_color( max, red_t()   ) - get_color( min, red_t()   ))
   , _green_range( get_color( max, green_t() ) - get_color( min, green_t() ))
   , _blue_range ( get_color( max, blue_t()  ) - get_color( min, blue_t()  ))
   {}

   template <typename P1, typename P2>
   void operator()(const P1& src, P2& dst) const
   {
      const double& red   = get_color( src, red_t()   );
      const double& green = get_color( src, green_t() );
      const double& blue  = get_color( src, blue_t()  );

      double red_dst   = red   / _red_range * 255.0;
      double green_dst = green / _green_range * 255.0;
      double blue_dst  = blue  / _blue_range * 255.0;

      get_color( dst, red_t() )   = static_cast<unsigned char>( red_dst   );
      get_color( dst, green_t() ) = static_cast<unsigned char>( green_dst );
      get_color( dst, blue_t() )  = static_cast<unsigned char>( blue_dst  );
   }

private:

   double _red_range;
   double _green_range;
   double _blue_range;
};

struct invert
{
   template < typename Channel> void operator()( const Channel& src 
                                               , Channel&       dst )
   const
   {
      dst = channel_traits< Channel >::max_value() - src;
   }
};

struct invert_pixel
{
   template < typename Pixel >
   Pixel operator()(const Pixel& src ) const
   {
      Pixel dst;
      static_for_each( src, dst, invert() );

      return dst;
   }
};

int main()
{
   TIFFSetErrorHandler  ( (TIFFErrorHandler) tiff_error_handler   );
   TIFFSetWarningHandler( (TIFFErrorHandler) tiff_warning_handler );

   {
      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );
      tiff_file_t file = boost::gil::detail::tiff_open_for_read( file_name );

      typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
      typedef image< rgb64f_pixel_t, true > rgb64f_planar_image_t;

      basic_tiff_image_read_info info;
      boost::gil::detail::read_image_info( file, info );

      rgb64f_planar_image_t src( info._width, info._height );

      tsize_t scanline_size = TIFFScanlineSize( file.get() );

      std::vector< nth_channel_view_type<rgb64f_planar_image_t::view_t>::type::value_type > buffer( scanline_size );

      for( tsample_t sample = 0; sample < 3; ++sample )
      {
         for( uint32 row = 0; row < info._height; ++row )
         {
            int ret = TIFFReadScanline( file.get()
                                      , &buffer.front()
                                      , row
                                      , sample             );

            assert( ret == 1 );

            // copy into view
            std::copy( buffer.begin()
                     , buffer.begin() + info._width
                     , nth_channel_view( view( src ), sample ).row_begin( row ));
         }
      }

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\caspian_3.bmp", const_view( dst ));

   }

   {
      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );
      tiff_file_t file = boost::gil::detail::tiff_open_for_read( file_name );

      typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
      typedef image< rgb64f_pixel_t, false > rgb64f_image_t;

      basic_tiff_image_read_info info;
      boost::gil::detail::read_image_info( file, info );

      rgb64f_image_t src( info._width, info._height );

      tsize_t scanline_size = TIFFScanlineSize( file.get() );
      std::vector< nth_channel_view_type<rgb64f_image_t::view_t>::type::value_type > buffer( scanline_size );

      for( tsample_t sample = 0
         ; sample < num_channels<rgb64f_pixel_t>::value
         ; ++sample                                      )
      {
         for( uint32 row = 0; row < info._height; ++row )
         {
            int ret = TIFFReadScanline( file.get()
                                      , &buffer.front()
                                      , row
                                      , sample             );

            assert( ret == 1 );

            // copy into view
            std::copy( buffer.begin()
                     , buffer.begin() + info._width
                     , nth_channel_view( view( src ), sample ).row_begin( row ));
         }
      }

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\caspian_4.bmp", const_view( dst ));

   }


   {
      // caspian.tif	279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

      typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
      typedef image< rgb64f_pixel_t, true > rgb64f_planar_image_t;
      rgb64f_planar_image_t src;

      read_image( file_name, src, tiff_tag() );

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\caspian.bmp", const_view( dst ));
   }

   {
      // cramps.tif	800x607 8-bit b&w (packbits) "cramps poster"

      string file_name( ".\\test_images\\tiff\\libtiffpic\\cramps.tif" );

      gray8_image_t src;
      read_image( file_name, src, tiff_tag() );

      tiff_photometric_interpretation::type value;
      get_property<string, tiff_photometric_interpretation>( file_name, value, tiff_tag() );

      if( value == PHOTOMETRIC_MINISWHITE )
      {
         gray8_image_t dst( view( src ).dimensions() );
         transform_pixels( const_view( src ), view( dst ), invert_pixel() );

         bmp_write_view( ".\\cramps.bmp", const_view( dst ));
      }
      else
      {
         bmp_write_view( ".\\cramps.bmp", const_view( src ));
      }
   }

   {
      // cramps-tile.tif 256x256 tiled version of cramps.tif (no compression)
      string file_name( ".\\test_images\\tiff\\libtiffpic\\cramps_tile.tif" );

      TIFF* file = TIFFOpen( file_name.c_str(), "r" );
      //ttile_t number_of_tiles = TIFFNumberOfTiles( file );
   }

   {
      // dscf0013.tif	640x480 YCbCr digital camera image which lacks Reference
		// Black/White values. Contains EXIF SubIFD. No compression.

      string file_name( ".\\test_images\\tiff\\libtiffpic\\dscf0013.tif" );
      TIFF* file = TIFFOpen( file_name.c_str(), "r" );
   }

   {
      // fax2d.tif	1728x1082 1-bit b&w (G3/2D) facsimile

      string file_name( ".\\test_images\\tiff\\libtiffpic\\fax2d.tif" );
      tiff_file_t file = boost::gil::detail::tiff_open_for_read( file_name );

      basic_tiff_image_read_info info;
      boost::gil::detail::read_image_info( file, info );

      tsize_t strip_size = TIFFStripSize     ( file.get() );
      tsize_t max_strips = TIFFNumberOfStrips( file.get() );

      std::size_t image_size_in_bytes = ( info._width / 8 ) * info._height;

      std::vector<unsigned char> buffer( image_size_in_bytes );

      unsigned int offset = 0;
      for( tsize_t strip_count = 0; strip_count < max_strips; ++strip_count )
      {
         int size = TIFFReadEncodedStrip( file.get()
                                        , strip_count
                                        , ( &buffer.front() ) + offset
                                        , strip_size                    );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }

      unsigned char* first_byte = &buffer.front();
      unsigned char* last_byte  = &buffer.front() + ( image_size_in_bytes - 1 );

      typedef bit_aligned_image1_type<1, gray_layout_t>::type image_t;
      typedef image_t::view_t view_t;

      view_t::x_iterator begin( first_byte, 0 );
      view_t::x_iterator end  ( last_byte , 7 );

      image_t src( info._width, info._height );
      std::copy( begin, end, view( src ).begin() );

      tiff_photometric_interpretation::type value;
      get_property<string, tiff_photometric_interpretation>( file_name, value, tiff_tag() );

      if( value == PHOTOMETRIC_MINISWHITE )
      {
         image_t inv_src( info._width, info._height );
         // @todo How to invert 1 bit image?
         // transform_pixels( const_view( src ), view( inv_src ), invert_pixel() );

         gray8_image_t dst( info._width, info._height );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\fax2d.bmp", view( dst ));
      }
      else
      {
         gray8_image_t dst( info._width, info._height );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\fax2d.bmp", view( dst ));
      }
   }
}