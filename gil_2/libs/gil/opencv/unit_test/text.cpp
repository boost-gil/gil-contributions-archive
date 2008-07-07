#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <boost\gil\extension\opencv\text.hpp>

#include <boost\gil\extension\io_new\png_all.hpp>

using namespace boost::gil;
using namespace boost::gil::opencv;

BOOST_AUTO_TEST_CASE( test_resize_nn )
{
    rgb8_image_t src;
    read_image( "..\\in\\in.png", src, png_tag() ); 

    ipl_font_wrapper font = create_ipl_font( Font_Hershey_Simplex()
                                           , 1.0
                                           , 1.0
                                           , 0
                                           , 1
                                           , four_connected_line()
                                           );

    putText( view( src )
           , std::string( "Hello World!" )
           , boost::gil::opencv::point_t( 10,10 )
           , font
           , rgb8_pixel_t( 0, 100, 88 )
           );
    
    
    write_view( "..\\out\\text.png", view( src ), png_tag() );
}

