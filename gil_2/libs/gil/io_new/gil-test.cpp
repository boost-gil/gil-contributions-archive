#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/pnm_read.hpp>
#include <boost/gil/extension/io_new/tiff_write.hpp>

#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    const std::string filename( ".\\test_images\\pnm\\p4.pnm" );

    typedef bit_aligned_image1_type< 1, gray_layout_t >::type image_t;
    //typedef gray8_image_t image_t;

    image_t img;
    read_image( filename, img, pnm_tag() );

    write_view( ".\\test\\pnm\\p4.tif", view( img ), tiff_tag() );

    return 0;
}
