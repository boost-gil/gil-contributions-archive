
#include <boost/gil/extension/io_new/bmp_all.hpp>
//#include <boost/gil/extension/io_new/jpeg_all.hpp>
//#include <boost/gil/extension/io_new/png_all.hpp>
//#include <boost/gil/extension/io_new/pnm_all.hpp>
//#include <boost/gil/extension/io_new/targa_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( cook_book )

BOOST_AUTO_TEST_CASE( recipe_1 )
{
    rgb8_image_t img;
    read_image( bmp_filename, img, tiff_tag() );

    read_view( bmp_filename, view( img ), tiff_tag() );
}

BOOST_AUTO_TEST_SUITE_END()
