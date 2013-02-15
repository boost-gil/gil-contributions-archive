#include "stdafx.h"

#include <boost/gil/gil_all.hpp>

#include <boost/integer.hpp>
#include <boost/integer/integer_mask.hpp>

#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( gil_test )

BOOST_AUTO_TEST_CASE( small_test )
{
    rgb8_image_t src( 10,10 );
    fill_pixels( view( src ), rgb8_image_t( 255, 0, 0 ));

    bgr8_image_t dst( 10,10 );
    fill_pixels( view( dst ), bgr8_image_t( 0, 0, 0 ));


    std::copy( view(src).begin(), view(src).end(), view( dst ).begin() );

    //typedef rgb8_image_t::view_t::x_iterator rgb_x_iterator_t;
    //typedef bgr8_image_t::view_t::x_iterator bgr_x_iterator_t;
    //
    //rgb_x_iterator_t oo;
    //bgr_x_iterator_t pp;

    //BOOST_STATIC_ASSERT(( is_same< t1_t, t2_t >::value ));
}

BOOST_AUTO_TEST_SUITE_END()
