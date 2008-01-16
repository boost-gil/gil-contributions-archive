#include "stdafx.h"

#include <iostream>
#include <boost/type_traits/is_same.hpp>
#include <boost/gil/extension/io_new_2/io.hpp>
#include <boost/gil/extension/io_new_2/png.hpp>
#include <boost/gil/extension/io_new_2/jpeg.hpp>
#include <boost/gil/gil_all.hpp>
#include <iterator>
#include <algorithm>

using namespace std;

template<typename ViewT>
void print_file( ViewT const& view, ostream & out )
{
    for(int i = 0; i != view.height(); ++ i )
    {
        for( typename ViewT::x_iterator it=view.row_begin(i), e=view.row_end(i); it != e;++it)
        {
            out << int(boost::gil::get_color(*it,boost::gil::gray_color_t())) << " ";
        }
        out << "\n";
    }
}

int main(int argc, char *argv[])
{
	using namespace boost::gil;
	using namespace boost::mpl;

   typedef bit_aligned_image_type<vector_c<int,1>,gray_layout_t>::type image_t;

   image_t img;
   print_file(view(img),std::cout);

    {
        rgb8_image_t image;
        read_image( "test\\test.PNG", image, png_tag());
    }

    {
        rgb8_image_t image;
        read_image( "test\\test.jpeg", image, jpeg_tag());
    }

    return 0;
}
