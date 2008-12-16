#include "stdafx.h"

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/pnm_all.hpp>


using namespace std;
using namespace boost::gil;

int main(int argc, char *argv[])
{
    string in ( ".\\test_images\\pnm\\p1.pnm" );
    string out( ".\\test\\pnm\\p1.pnm" );

    gray8_image_t img;
    read_image( in, img, pnm_tag() );
    write_view( out, view( img ), pnm_tag() );
}
