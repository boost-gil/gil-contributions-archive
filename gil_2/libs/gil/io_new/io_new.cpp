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

typedef image< rgb64f_pixel_t, false > rgb64f_image_t;
typedef rgb64f_image_t::view_t rgb64f_view_t;


template < typename Pixel1
         , typename Pixel2
         >
struct my_color_converter_impl
{
    void operator()( const Pixel1& src
                   , Pixel2&       dst
                   , double red_range
                   , double green_range
                   , double blue_range ) const
    {
    }
};

template <>
struct my_color_converter_impl< rgb64f_pixel_t
                              , rgb8_pixel_t
                              >
{
    void operator()( const rgb64f_pixel_t& src
                   , rgb8_pixel_t&         dst
                   , double red_range
                   , double green_range
                   , double blue_range ) const
    {
      typedef channel_type< rgb8_pixel_t >::type channel_t;
      double max = static_cast<double>( channel_traits<channel_t>::max_value() );

      const double& red   = get_color( src, red_t()   );
      const double& green = get_color( src, green_t() );
      const double& blue  = get_color( src, blue_t()  );

      double red_dst   = red   / red_range * max;
      double green_dst = green / green_range * max;
      double blue_dst  = blue  / blue_range * max;

      get_color( dst, red_t()   ) = static_cast<channel_t>( red_dst   );
      get_color( dst, green_t() ) = static_cast<channel_t>( green_dst );
      get_color( dst, blue_t()  ) = static_cast<channel_t>( blue_dst  );
    }
};

struct my_color_converter {

   my_color_converter() {}

   my_color_converter( const rgb64f_pixel_t& min
                     , const rgb64f_pixel_t& max ) 
   : _red_range  ( get_color( max, red_t()   ) - get_color( min, red_t()   ))
   , _green_range( get_color( max, green_t() ) - get_color( min, green_t() ))
   , _blue_range ( get_color( max, blue_t()  ) - get_color( min, blue_t()  ))
   {}

   template< typename SrcP
           , typename DstP
           >
   void operator()(const SrcP& src,DstP& dst) const
   { 
      my_color_converter_impl< SrcP
                             , DstP >()( src
                                       , dst
                                       , _red_range
                                       , _green_range
                                       , _blue_range   );
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

void read_test();
void read_and_convert_test();
void write_test();

int main()
{
   TIFFSetErrorHandler  ( (TIFFErrorHandler) tiff_error_handler   );
   TIFFSetWarningHandler( (TIFFErrorHandler) tiff_warning_handler );

   //read_test();
   //read_and_convert_test();
   write_test();

}

void read_test()
{
   // read view tests

   {
      // basic read view test

      string file_name( ".\\test.tiff" );
      basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

      rgb8_image_t src( info._width, info._height );
      read_view( file_name, view( src ), tiff_tag() );

      bmp_write_view( ".\\basic_read_view_test.bmp", const_view( src ));
   }


   {
      // basic read view test

      string file_name( ".\\test_images\\tiff\\found online\\flower.tif" );
      basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

      rgb8_image_t src( info._width, info._height );
      read_view( file_name, view( src ), tiff_tag() );

      bmp_write_view( ".\\basic_read_view_test.bmp", const_view( src ));
   }

   {
      // partial read view test

      string file_name( ".\\test_images\\tiff\\found online\\flower.tif" );
      basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

      point_t top_left( 19, 88 );

      rgb8_image_t src( point_t( info._width, info._height ) - top_left );
      read_view( file_name, view( src ), top_left, tiff_tag() );

      bmp_write_view( ".\\partial_read_view_test.bmp", const_view( src ));
   }


   {
      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

      basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

      rgb64f_image_t src( info._width - 20, info._height - 20 );

      read_view( file_name, view( src ), point_t( 20, 20 ), tiff_tag() );

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\read_view_test.bmp", const_view( dst ));
   }

   {
      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

      basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

      rgb64f_image_t src( info._width, info._height );

      read_view( file_name, view( src ), tiff_tag() );

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\read_view_test.bmp", const_view( dst ));
   }

   {
      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );
      tiff_file_t file = boost::gil::detail::tiff_open_for_read( file_name );

      rgb64f_image_t src;

      read_image( file_name, src, tiff_tag() );

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t dst( view( src ).dimensions() );
      copy_and_convert_pixels( view( src ), view( dst ), my_color_converter( min, max ) );
   
      bmp_write_view( ".\\caspian_interleaved.bmp", const_view( dst ));
   }

   {
      // caspian.tif	279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

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
   
      bmp_write_view( ".\\caspian_planar.bmp", const_view( dst ));
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
      string file_name( ".\\test_images\\tiff\\libtiffpic\\fax2d.tif" );

      typedef bit_aligned_image1_type<1, gray_layout_t>::type image_t;

      image_t src;
      read_image( file_name, src, tiff_tag() );

      tiff_photometric_interpretation::type value;
      get_property<string, tiff_photometric_interpretation>( file_name, value, tiff_tag() );

      if( value == PHOTOMETRIC_MINISWHITE )
      {
         image_t inv_src( view( src ).dimensions() );
         // @todo How to invert 1 bit image?
         // transform_pixels( const_view( src ), view( inv_src ), invert_pixel() );

         gray8_image_t dst( view( src ).dimensions() );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\fax2d_2.bmp", view( dst ));
      }
      else
      {
         gray8_image_t dst( view( src ).dimensions() );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\fax2d_2.bmp", view( dst ));
      }
   }

   {
      //g3test.tif	TIFF equivalent of g3test.g3 created by fax2tiff
      string file_name( ".\\test_images\\tiff\\libtiffpic\\g3test.tif" );

      typedef bit_aligned_image1_type<1, gray_layout_t>::type image_t;

      image_t src;
      read_image( file_name, src, tiff_tag() );

      tiff_photometric_interpretation::type value;
      get_property<string, tiff_photometric_interpretation>( file_name, value, tiff_tag() );

      if( value == PHOTOMETRIC_MINISWHITE )
      {
         image_t inv_src( view( src ).dimensions() );
         // @todo How to invert 1 bit image?
         // transform_pixels( const_view( src ), view( inv_src ), invert_pixel() );

         gray8_image_t dst( view( src ).dimensions() );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\g3test.bmp", view( dst ));
      }
      else
      {
         gray8_image_t dst( view( src ).dimensions() );
         copy_and_convert_pixels( view( src ), view( dst ) );

         bmp_write_view( ".\\g3test.bmp", view( dst ));
      }
   }

   {
      //jello.tif	256x192 8-bit RGB (lzw palette) Paul Heckbert "jello"
      string file_name( ".\\test_images\\tiff\\libtiffpic\\jello.tif" );
      /*
      image_t src;
      read_image( file_name, src, tiff_tag() );

      rgb8_image_t src;
      bmp_write_view( ".\\jello.bmp", view( src ));
      */
   }
}

void read_and_convert_test()
{
   {
      // test default color converter

      string file_name( ".\\test_images\\tiff\\found online\\flower.tif" );

      gray8_image_t src;
      read_and_convert_image( file_name, src, tiff_tag() );

      bmp_write_view( ".\\default_cc_test.bmp", const_view( src ));
   }

   {
      // test own color converter

      // caspian.tif 279x220 64-bit floating point (deflate) Caspian Sea from space

      string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

      rgb64f_pixel_t min( 0.0
                        , 0.0
                        , 0.0 );

      rgb64f_pixel_t max( 1000.0
                        , 1000.0
                        , 1000.0 );

      rgb8_image_t src;
      read_and_convert_image( file_name, src, my_color_converter( min, max ), tiff_tag() );

      bmp_write_view( ".\\own_cc_test.bmp.bmp", const_view( src ));
   }
}

void write_test()
{
   {
      rgb8_image_t src( 100, 100 );
      fill_pixels( view( src ), rgb8_pixel_t( 255, 0, 0 ));

      write_view( std::string( ".\\write_interleaved_image.tiff" ), const_view( src ), tiff_tag() );
   }

   {
      rgb8_planar_image_t src( 1000, 1000 );
      fill_pixels( view( src ), rgb8_pixel_t( 255, 0, 0 ));

      write_view( std::string( ".\\write_planar_image.tiff" ), const_view( src ), tiff_tag() );
   }

}
