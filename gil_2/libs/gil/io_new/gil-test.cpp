#include "stdafx.h"

#include <iostream>
#include <boost/type_traits/is_same.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>
#include <boost/gil/gil_all.hpp>
#include <iterator>
#include <algorithm>

using namespace std;
using namespace boost;
using namespace gil;

typedef pixel<double, gray_layout_t> gray64f_pixel_t;
typedef image< gray64f_pixel_t, false > gray64f_image_t;

typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
typedef image< rgb64f_pixel_t, false > rgb64f_image_t;

typedef pixel< double, rgba_layout_t> rgba64f_pixel_t;
typedef image< rgba64f_pixel_t, false > rgba64f_image_t;

template< int Value >
struct compare
{
   template< typename Channel >
   void operator() ( const Channel& c )
   {
      if( c != 0 )
      {
         throw std::exception();
      }
   }
};


template< int Value >
struct eval_to
{
   template< typename Pixel >
   void operator() ( const Pixel& p )
   {
      static_for_each( p, compare<Value>() );
   }
};

template< typename Image
        , typename String
        >
void read_test_impl( const String& file_name )
{
   Image src;
   read_image( file_name, src, tiff_tag() );

   assert( view( src ).dimensions() == point_t( 100, 100 ));
   for_each_pixel( view( src ), eval_to<0>() );
}

int main(int argc, char *argv[])
{
/*   
   using namespace boost::gil;
   using namespace boost::mpl;
   namespace fs = boost::filesystem;

   string in_path( ".\\test_images\\png\\test.png" );
   string out_path( ".\\test\\png\\test.png" );

   {
      gray8_image_t img;
      read_and_convert_image( in_path, img, png_tag() );
      write_view( out_path, view( img ), png_tag() );

      for( int x = 0; x < view( img ).width(); ++x )
      {
         unsigned char v = *view( img ).at( x, 0 );

         int i = 9;
      }

   }

   {
      gray8_image_t img;
      read_and_convert_image( in_path, img, jpeg_tag() );
      write_view( out_path, view( img ), jpeg_tag() );

      for( int x = 0; x < view( img ).width(); ++x )
      {
         unsigned char v = *view( img ).at( x, 0 );

         int i = 9;
      }

   }

   {
      gray8_image_t img;
      read_and_convert_image( in_path, img, tiff_tag() );
      write_view( out_path, view( img ), tiff_tag() );

      for( int x = 0; x < view( img ).width(); ++x )
      {
         unsigned char v = *view( img ).at( x, 0 );

         int i = 9;
      }

   }
*/
/*

   fs::path in_full_path  = fs::system_complete( fs::path( in_path, fs::native ) );

   if ( fs::is_directory( full_path ) )
   {
      fs::directory_iterator end_iter;
      for( fs::directory_iterator dir_itr( full_path )
         ; dir_itr != end_iter
         ; ++dir_itr
         )
      {
         if ( fs::is_regular( dir_itr->status() ) 
            && ( fs::extension( dir_itr->path() ) == ".PNG" ))
         {
            rgb8_image_t img;
            read_and_convert_image( in_path + dir_itr->path().leaf(), img, png_tag() );

            write_view( out_path + fs::basename( dir_itr->path() ) + ".png"
                      , view( img )
                      , png_tag()
                      );
         }
      }
   }
*/
/*
   {
      string file_name( ".\\test_images\\tiff\\gray1_image.tif" );
      typedef bit_aligned_image1_type< 1, gray_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray2_image.tif" );
      typedef bit_aligned_image1_type< 2, gray_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray4_image.tif" );
      typedef bit_aligned_image1_type< 4, gray_layout_t >::type image_t;

      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray8_image.tif" );
      read_test_impl<gray8_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray16_image.tif" );
      read_test_impl<gray16_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray32_image.tif" );
      read_test_impl<gray32_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\gray64f_image.tif" );
      read_test_impl<gray64f_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb1_image.tif" );
      typedef bit_aligned_image3_type< 1, 1, 1, rgb_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb2_image.tif" );
      typedef bit_aligned_image3_type< 2, 2, 2, rgb_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb4_image.tif" );
      typedef bit_aligned_image3_type< 4, 4, 4, rgb_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb8_image.tif" );
      read_test_impl<rgb8_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb16_image.tif" );
      read_test_impl<rgb16_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb32_image.tif" );
      read_test_impl<rgb32_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgb64f_image.tif" );
      read_test_impl<rgb64f_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba1_image.tif" );
      typedef bit_aligned_image4_type< 1, 1, 1, 1, rgba_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba2_image.tif" );
      typedef bit_aligned_image4_type< 2, 2, 2, 2, rgba_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba4_image.tif" );
      typedef bit_aligned_image4_type< 4, 4, 4, 4, rgba_layout_t >::type image_t;
      read_test_impl<image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba8_image.tif" );
      read_test_impl<rgba8_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba16_image.tif" );
      read_test_impl<rgba16_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba32_image.tif" );
      read_test_impl<rgba32_image_t>( file_name );
   }

   {
      string file_name( ".\\test_images\\tiff\\rgba64f_image.tif" );
      read_test_impl<rgba64f_image_t>( file_name );
   }

   {
      image_read_info< tiff_tag > info = read_image_info( "test\\test.tif", tiff_tag() );

      const string str( "test\\test.tif" );

      rgba8_image_t img;
      read_test_impl<rgba8_image_t>( str );
   }

   {
      image_read_info< png_tag > info = read_image_info( "test\\test.png", png_tag() );

      rgba8_image_t img;
      read_image( "test\\test.png", img, png_tag() );
   }

   {
      image_read_info< jpeg_tag > info = read_image_info( "test\\test.jpg", jpeg_tag() );
   }
*/
   return 0;
}
