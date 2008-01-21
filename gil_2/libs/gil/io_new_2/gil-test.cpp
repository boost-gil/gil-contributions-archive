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
      rgb8_image_t image;
      read_image( "test\\test.tif", image, tiff_tag());
   }

/*
   {
      rgb8_image_t image;
      read_image( "test\\test.PNG", image, png_tag());
   }

   {
      rgb8_image_t image;
      read_image( "test\\test.jpeg", image, jpeg_tag());
   }
*/
    return 0;
}
