#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/pnm_read.hpp>
#include <boost/gil/extension/io_new/tiff_write.hpp>

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

using namespace std;
using namespace boost;
using namespace gil;

struct swap_
{
    template< typename Pixel >
    void operator() ( Pixel& p )
    {
        byte_t& b = semantic_at_c<0>( p );

        if( b != 255 && b != 0 )
            int i = 9;

        gil::detail::swap_bits( b );
    }
};

int main(int argc, char *argv[])
{
    string in ( ".\\test_images\\pnm\\p4.pnm" );

    typedef bit_aligned_image1_type< 1, gray_layout_t >::type image_t;

    image_t img;
    read_and_convert_image( in, img, pnm_tag() );

/*
    gray8_image_t dst( img.dimensions() );
    copy_and_convert_pixels( view( img ), view( dst ), my_converter() );
*/

    write_view( ".\\test\\pnm\\pnm4.tif"
              , view( img )
              , tiff_tag()
              );

   return 0;
}
