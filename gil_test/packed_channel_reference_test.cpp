#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( gil_test )

template< typename Base, typename ChannelSizes >
void test()
{
	typedef packed_channel_reference< typename Base
                                    , 0
                                    , mpl::at_c< ChannelSizes, 0 >::type::value
                                    , true
                                    > channel1_reference_t;

    typedef packed_channel_reference< typename Base
                                    , mpl::at_c< ChannelSizes, 0 >::type::value
                                    , mpl::at_c< ChannelSizes, 1 >::type::value
                                    , true
                                    > channel2_reference_t;

	typedef packed_channel_reference< typename Base
                                    , mpl::at_c< ChannelSizes, 0 >::type::value + mpl::at_c< ChannelSizes, 1 >::type::value
                                    , mpl::at_c< ChannelSizes, 2 >::type::value
                                    , true
                                    > channel3_reference_t;

	Base data = 0;
	channel1_reference_t channel1( &data );
	channel2_reference_t channel2( &data );
    channel3_reference_t channel3( &data );

	channel1 = channel_traits< channel1_reference_t >::max_value();
	channel2 = channel_traits< channel2_reference_t >::max_value();
	channel3 = channel_traits< channel3_reference_t >::max_value();

    BOOST_CHECK_EQUAL( data, channel_traits< typename Base >::max_value() );
}

BOOST_AUTO_TEST_CASE( packed_channel_reference_test )
{
    // use BOOST_PP_REPEAT_FROM_TO for complete test
    test< bits8, mpl::vector3_c< unsigned, 3, 3, 2 > >();
    test< bits16, mpl::vector3_c< unsigned, 5, 5, 6 > >();
    test< bits32, mpl::vector3_c< unsigned, 11, 11, 10 > >();
    test< bits64, mpl::vector3_c< unsigned, 21, 21, 22 > >();
}

BOOST_AUTO_TEST_SUITE_END()
