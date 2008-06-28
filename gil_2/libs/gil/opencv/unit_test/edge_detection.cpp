#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <boost\gil\extension\opencv\edge_detection.hpp>

#include <boost\gil\extension\io_new\png_all.hpp>

using namespace boost::gil;
using namespace boost::gil::opencv;

BOOST_AUTO_TEST_CASE( test_sobel )
{
    gray8_image_t src;
    read_and_convert_image( "..\\in\\in.png", src, png_tag() ); 

    gray8_image_t dst( view( src ).dimensions() );

    sobel( view( src )
         , view( dst )
         , aperture3()
         );

    write_view( "..\\out\\sobel.png", view( dst ), png_tag() );
}

BOOST_AUTO_TEST_CASE( test_laplace )
{
    rgb8_image_t src;
    read_image( "..\\in\\in.png", src, png_tag() ); 

    rgb32f_image_t dst( view( src ).dimensions() );

    laplace( view( src )
           , view( dst )
           , aperture3()
           );

    write_view( "..\\out\\laplace.png"
              , color_converted_view< rgb8_pixel_t >( view( dst ))
              , png_tag()
              );
}

BOOST_AUTO_TEST_CASE( test_canny )
{
    gray8_image_t src;
    read_and_convert_image( "..\\in\\in.png", src, png_tag() ); 

    gray8_image_t edges( view( src ).dimensions() );

    canny( view( src   )
         , view( edges )
         , 60
         , 180
         , aperture3()
         );

    write_view( "..\\out\\canny.png", view( edges ), png_tag() );
}