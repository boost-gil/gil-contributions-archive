#include "stdafx.h"

#include <iostream>

#include <boost/gil/extension/io_new/bmp_all.hpp>

#include <boost/gil/extension/io_new/png_write.hpp>
#include <boost/gil/gil_all.hpp>

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
    const std::string filename( ".\\test_images\\bmp\\g32bf.bmp" );

    typedef rgb8_image_t image_t;

    image_t img;
    read_and_convert_image( filename, img, my_color_converter(), bmp_tag() );

    write_view( ".\\test\\bmp\\test.png", view( img ), png_tag() );

    return 0;
}
