#include "stdafx.h"

#include "boost/gil/extension/io_new/jpeg_io.hpp"

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
   read_image( file_name, src, jpeg_tag() );
}

void basic_read_test()
{
   {
      string file_name( ".\\test_images\\jpeg\\test.jpg" );
      read_test_impl<image_t>( file_name );
   }
}