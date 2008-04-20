#include "stdafx.h"

#include <iostream>

#include <boost/gil/extension/io_new/tiff_read.hpp>
#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    string filename( ".\\test_images\\tiff\\test.tif" );

    {
        rgb8_image_t img;
        read_image( filename, img, tiff_tag() );
    }

   return 0;
}
