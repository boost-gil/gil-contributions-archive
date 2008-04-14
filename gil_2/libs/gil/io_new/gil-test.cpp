#include "stdafx.h"

#include <iostream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_read.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    string filename( ".\\test_images\\tiff\\test.tif" );

    rgb8_image_t img( 200, 133 );
    read_and_convert_view( filename, view( img ), tiff_tag() );

   return 0;
}
