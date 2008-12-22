#include "stdafx.h"

#include <boost/filesystem.hpp>

#define ADD_FS_PATH_SUPPORT
#define ENABLE_GRAY_ALPHA

#include <boost/gil/extension/io_new/pnm_all.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

int main()
{
    const string filename( ".\\test_images\\pnm\\p4.pnm" );

    {
        rgb8_image_t img;
        read_and_convert_image( filename, img, pnm_tag() );
    }
}
