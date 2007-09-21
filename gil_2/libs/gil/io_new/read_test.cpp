#include "stdafx.h"

#include "boost/gil/extension/io_new/tiff_io.hpp"

#include "read_test.hpp"

using namespace std;
using namespace boost;
using namespace gil;
using namespace fusion;

typedef pixel<double, gray_layout_t> gray64f_pixel_t;
typedef image< gray64f_pixel_t, false > gray64f_image_t;

typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
typedef image< rgb64f_pixel_t, false > rgb64f_image_t;

typedef pixel< double, rgba_layout_t> rgba64f_pixel_t;
typedef image< rgba64f_pixel_t, false > rgba64f_image_t;

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

void read_test()
{
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
}