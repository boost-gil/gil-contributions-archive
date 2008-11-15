#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/bmp_io_old.hpp>
#include <boost/gil/extension/io_new/jpeg_io_old.hpp>
#include <boost/gil/extension/io_new/pnm_io_old.hpp>
#include <boost/gil/extension/io_new/png_io_old.hpp>
#include <boost/gil/extension/io_new/tiff_io_old.hpp>

using namespace std;
using namespace boost;
using namespace gil;

struct my_color_converter
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        get_color( dst, red_t()   ) = get_color( src, red_t()   );
        get_color( dst, green_t() ) = get_color( src, green_t() );
        get_color( dst, blue_t()  ) = get_color( src, blue_t()  );
    }
};

int main(int argc, char *argv[])
{
    std::ofstream o( "\\test.txt" );

    const std::string filename( ".\\test_images\\bmp\\g08p64.bmp" );

    typedef gray8_image_t image_t;

/*
    image_read_settings< bmp_tag > settings( point_t( 0, 5  )
                                           , point_t( 30, 30 )
                                           );
*/

    image_t img;
    //read_image( filename, img, settings );
    read_image( filename, img, bmp_tag() );

    write_view( ".\\test\\bmp\\test8.bmp", view( img ), bmp_tag() );

    return 0;
}
