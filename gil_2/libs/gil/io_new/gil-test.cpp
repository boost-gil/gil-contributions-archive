#include "stdafx.h"

#include <boost/gil/extension/io_new/pnm_read.hpp>

using namespace std;
using namespace boost::gil;

int main()
{
    rgb8_image_t img;
    read_image( ".", img, pnm_tag() );
}

