#include "stdafx.h"

#include <boost/gil/extension/io_new/bmp_read.hpp>

using namespace boost::gil;

int main()
{
    rgb8_image_t img;
    read_and_convert_view( "test.bmp", view( img ), bmp_tag() );
}
