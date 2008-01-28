#include "stdafx.h"

#include <iostream>
#include <boost/type_traits/is_same.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/png.hpp>
#include <boost/gil/extension/io_new_2/jpeg.hpp>
#include <boost/gil/extension/io_new_2/tiff.hpp>
#include <boost/gil/gil_all.hpp>
#include <iterator>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
   using namespace boost::gil;
   using namespace boost::mpl;

   {
      image_read_info< tiff_tag > info = read_image_info( "test\\test.tif", tiff_tag() );

      rgba8_image_t img;
      read_image( "test\\test.tif", img, tiff_tag() );
   }

   {
      image_read_info< png_tag > info = read_image_info( "test\\test.png", png_tag() );

      rgba8_image_t img;
      read_image( "test\\test.png", img, png_tag() );
   }

   {
      image_read_info< jpeg_tag > info = read_image_info( "test\\test.jpg", jpeg_tag() );
   }

   return 0;
}
