#include "stdafx.h"

#include <iostream>

#include <boost/gil/extension/io_new/png_read.hpp>
#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
   rgb8_image_t img;
   read_and_convert_image( "", img, png_tag() );

   return 0;
}
