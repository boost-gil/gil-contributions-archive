#include "stdafx.h"

#define ADD_FS_PATH_SUPPORT
#define ENABLE_GRAY_ALPHA

#include <iostream>
#include <boost/gil/extension/io_new/png_all.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

typedef png_tag tag_t;

int main()
{
    std::string filename( ".\\test_images\\png\\PngSuite\\BASI0G01.PNG" );
    rgba8_image_t img;
    read_and_convert_image( filename, img, tag_t() );
}
