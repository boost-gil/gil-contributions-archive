
#include <boost/gil/extension/io_new/bmp_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( cook_book )

BOOST_AUTO_TEST_CASE( recipe_1 )
{
    rgb8_image_t img;
    read_image( bmp_filename, img, bmp_tag() );    
}

BOOST_AUTO_TEST_SUITE_END()
