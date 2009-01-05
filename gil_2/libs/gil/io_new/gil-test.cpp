#include "stdafx.h"

#include <boost/gil/extension/io_new/jpeg_write.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

int main()
{
    std::string filename( "image.jpg" );
    rgb8_image_t img( 640, 480 );

    write_view( filename
              , view( img )
              , image_write_info< jpeg_tag >( 95 )
              );

}
