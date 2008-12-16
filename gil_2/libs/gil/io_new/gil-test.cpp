#include "stdafx.h"

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/pnm_all.hpp>


using namespace std;
using namespace boost::gil;

int main(int argc, char *argv[])
{
    LARGE_INTEGER beg;
    QueryPerformanceCounter( &beg );

    LARGE_INTEGER fre;
    QueryPerformanceFrequency ( &fre );

    string in ( ".\\test_images\\pnm\\p1.pnm" );
    string out( ".\\test\\pnm\\p1.pnm" );

    gray8_image_t img;
    read_image( in, img, pnm_tag() );
    write_view( out, view( img ), pnm_tag() );

    LARGE_INTEGER end;
    QueryPerformanceCounter( &end );

    __int64 dur =  end.QuadPart - beg.QuadPart;
}
