#include "stdafx.h"

#include <iostream>

#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/extension/io_new/png_write.hpp>
#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    const std::string filename( ".\\test_images\\bmp\\g01bg.bmp" );

    rgb8_image_t img;
    read_image( filename, img, bmp_tag() );

    write_view( ".\\test\\bmp\\test.png", view( img ), png_tag() );

    return 0;
}
