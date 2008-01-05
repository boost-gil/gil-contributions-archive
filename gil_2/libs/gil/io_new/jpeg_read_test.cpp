#include "stdafx.h"

#include "boost/gil/extension/io_new/jpeg_io.hpp"

using namespace std;
using namespace boost;
using namespace gil;

template< typename Image
        , typename String
        >
void jpeg_read_test_impl( const String& file_name )
{
   Image src;
   read_image( file_name, src, jpeg_tag() );
}

void basic_jpeg_read_test()
{
   {
      string file_name( ".\\test_images\\jpeg\\test.jpg" );
      jpeg_read_test_impl<rgb8_image_t>( file_name );
   }
}