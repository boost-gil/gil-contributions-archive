#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/toolbox/metafunctions/get_num_bits.hpp>

#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( gil_test )

//
// packed_channel_value<12> represents a 12 bit value type.
//
// These tests make sure that a value is represented correctly.
//

template< int K >
void test_packed_channel()
{
    typedef packed_channel_value<K> bits_t;

    BOOST_CHECK_EQUAL( channel_traits<bits_t>::min_value(),                                         0 );
    BOOST_CHECK_EQUAL( channel_traits<bits_t>::max_value(),                ( uint64_t( 1 ) << K ) - 1 );
    BOOST_CHECK_EQUAL( gil::detail::unsigned_integral_max_value<bits_t>::value, ( uint64_t( 1 ) << K ) - 1 );

    BOOST_CHECK_EQUAL( sizeof( bits_t ), K / 8 + 1 );
    BOOST_STATIC_ASSERT(( boost::is_integral< bits_t >::value ));

    {
        typedef packed_channel_value< K > bits_t;
        bits_t::value_type v = bits_t::min_value();
    }

    {
        unsigned int v = 10;

        typedef packed_channel_value< 5 > bits_t;
        bits_t b( v );

        bits_t::integer_t v2 = b;
        BOOST_CHECK_EQUAL( v, v2 );
    }

    {
        typedef packed_channel_value< 12 > bits_t;
        bits_t a(5), b(5), c;

        c = channel_multiply( a, b );

        BOOST_CHECK_EQUAL( channel_multiply( bits_t( 5 ), bits_t( 5 ) ), bits_t( 0 ) );
    }

    {
        unsigned int value = 0;
        
        typedef packed_channel_reference< unsigned short, 0, 5, 1 > channel_t;
        channel_t c( &value );
        c = channel_traits<channel_t>::min_value();
    }
}

BOOST_AUTO_TEST_CASE( packed_channel_test )
{
    // use BOOST_PP_REPEAT_FROM_TO for complete test
    test_packed_channel<  1 >();
    test_packed_channel<  7 >();
    test_packed_channel< 24 >();
    test_packed_channel< 31 >();
    test_packed_channel< 63 >();

    mpl::false_ oo;
    mpl::true_ pp;
    auto ll = is_integral< unsigned char >::type();
    auto kk = mpl::not_< is_class< unsigned char > >::type();
}

BOOST_AUTO_TEST_SUITE_END()
