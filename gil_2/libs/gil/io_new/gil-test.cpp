#include "stdafx.h"

#include <boost/filesystem.hpp>

#include <boost/gil/extension/io_new/tiff_all.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

int main()
{
    std::string filename( ".\\test_images\\tiff\\libtiffpic\\depth\\flower-minisblack-02.tif" );

    {
        typedef bit_aligned_image1_type< 2, gray_layout_t >::type image_t;
        image_t img;

        read_image( filename
                  , img
                  , tiff_tag()
                  );

        write_view( ".\\test\\tiff\\test4.tif"
                  , view( img )
                  , tiff_tag()
                  );
    }
}
