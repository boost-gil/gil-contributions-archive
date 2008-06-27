#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <boost\gil\extension\opencv\edge_detection.hpp>

#include <boost\gil\extension\io_new\png_all.hpp>

using namespace boost::gil;
using namespace boost::gil::opencv;

BOOST_AUTO_TEST_CASE( test_sobel )
{
    rgb8_image_t src;
    read_image( "..\\in\\in.png", src, png_tag() ); 

    rgb8_image_t dst( view( src ).dimensions() );

    sobel( view( src )
         , view( dst ) );

    write_view( "..\\out\\sobel.png", view( dst ), png_tag() );
}

BOOST_AUTO_TEST_CASE( test_laplace )
{
    rgb8_image_t src;
    read_image( "..\\in\\in.png", src, png_tag() ); 

    rgb32f_image_t dst( view( src ).dimensions() );

    laplace( view( src )
           , view( dst ) );

    write_view( "..\\out\\laplace.png"
              , color_converted_view< rgb8_pixel_t >( view( dst ))
              , png_tag()
              );
}