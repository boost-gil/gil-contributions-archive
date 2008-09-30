#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <boost\gil\extension\opencv\ipl_image_wrapper.hpp>

#include <boost\gil\extension\io_new\png_write.hpp>

using namespace boost::gil;
using namespace boost::gil::opencv;

BOOST_AUTO_TEST_CASE( test_ipl_image )
{
    rgb8_image_t img( 640, 480 );
    fill_pixels( view( img ), rgb8_pixel_t( 255, 255, 255 ) );

    ipl_image_wrapper ipl_img = create_ipl_image( view( img ));

    IplImage* ipl_img_p = ipl_img.get();

    rgb8_view_t v = interleaved_view( 640, 480
                                    , (rgb8_pixel_t*) ipl_img_p->imageData
                                    , 640 * 3
                                    );

    write_view( "..\\out\\ipl_image.png"
              , v
              , png_tag()
              );

    return;
}

BOOST_AUTO_TEST_CASE( test_value_semantics )
{
    rgb8_image_t img( 640, 480 );
    fill_pixels( view( img ), rgb8_pixel_t( 255, 255, 255 ) );

    ipl_image_wrapper ipl_img = create_ipl_image( view( img ));

    ipl_image_wrapper ipl_img_2( ipl_img );

    return;
}