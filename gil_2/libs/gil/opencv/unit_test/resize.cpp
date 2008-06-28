#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <boost\gil\extension\opencv\resize.hpp>

#include <boost\gil\extension\io_new\png_all.hpp>

using namespace boost::gil;
using namespace boost::gil::opencv;

BOOST_AUTO_TEST_CASE( test_resize )
{
    rgb8_image_t src;
    read_image( "..\\in\\in.png", src, png_tag() ); 

    rgb8_image_t dst( 640, 480 );

    resize( view( src )
          , view( dst )
          , nearest_neigbor() );

    write_view( "..\\out\\resize.png", view( dst ), png_tag() );
}