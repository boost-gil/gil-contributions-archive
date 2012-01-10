#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( gil_test )

template< int K >
void test_packed_channel()
{
    typedef packed_channel_value<K> bits_t;

    BOOST_CHECK_EQUAL(channel_traits<bits_t>::min_value(),                                                 0 );
    BOOST_CHECK_EQUAL(channel_traits<bits_t>::max_value(),                ( unsigned __int64( 1 ) << K ) - 1 );
    BOOST_CHECK_EQUAL(detail::unsigned_integral_max_value<bits_t>::value, ( unsigned __int64( 1 ) << K ) - 1 );

    BOOST_CHECK_EQUAL( sizeof( bits_t ), K / 8 + 1 );
    BOOST_STATIC_ASSERT(( boost::is_integral< bits_t >::value ));
}

BOOST_AUTO_TEST_CASE( packed_channel_test )
{
    // use BOOST_PP_REPEAT_FROM_TO for complete test
    test_packed_channel<  1 >();
    test_packed_channel<  7 >();
    test_packed_channel< 24 >();
    test_packed_channel< 31 >();
    test_packed_channel< 63 >();
}

BOOST_AUTO_TEST_SUITE_END()
