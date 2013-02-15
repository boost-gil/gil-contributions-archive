#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( gil_test )

//
// packed_channel_reference< bits8, 0, 4, true > - mutable half byte reference type
//
// The acutal values are stored somewhere else, like inside an integer.
//
// These tests make sure that a value is represented correctly.
//
template< typename Base, typename ChannelSizes >
void mutable_test()
{
    // Define a compile time the position and size of the channel.
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

    // the actual value
	Base data = 0;

    // create and assign
	channel1_reference_t channel1( &data );
	channel2_reference_t channel2( &data );
    channel3_reference_t channel3( &data );

	channel1 = channel_traits< channel1_reference_t >::max_value();
	channel2 = channel_traits< channel2_reference_t >::max_value();
	channel3 = channel_traits< channel3_reference_t >::max_value();

    BOOST_CHECK_EQUAL( data, channel_traits< Base >::max_value() );
}


//
// Pretty much the same test than above but without changing 
// the source value.
//
template< typename Base, typename ChannelSizes >
void immutable_test()
{
	typedef packed_channel_reference< typename Base
                                    , 0
                                    , mpl::at_c< ChannelSizes, 0 >::type::value
                                    , false
                                    > channel1_reference_t;

    typedef packed_channel_reference< typename Base
                                    , mpl::at_c< ChannelSizes, 0 >::type::value
                                    , mpl::at_c< ChannelSizes, 1 >::type::value
                                    , false
                                    > channel2_reference_t;

	typedef packed_channel_reference< typename Base
                                    , mpl::at_c< ChannelSizes, 0 >::type::value + mpl::at_c< ChannelSizes, 1 >::type::value
                                    , mpl::at_c< ChannelSizes, 2 >::type::value
                                    , false
                                    > channel3_reference_t;

    Base data = channel_traits< typename Base >::max_value();
	channel1_reference_t channel1( &data );
	channel2_reference_t channel2( &data );
    channel3_reference_t channel3( &data );

    BOOST_CHECK_EQUAL( channel1, channel_traits< channel1_reference_t >::max_value() );
    BOOST_CHECK_EQUAL( channel2, channel_traits< channel2_reference_t >::max_value() );
    BOOST_CHECK_EQUAL( channel3, channel_traits< channel3_reference_t >::max_value() );
}

BOOST_AUTO_TEST_CASE( packed_channel_reference_test )
{
    // use BOOST_PP_REPEAT_FROM_TO for complete test
    mutable_test< bits8, mpl::vector3_c< unsigned, 3, 3, 2 > >();
    mutable_test< bits16, mpl::vector3_c< unsigned, 5, 5, 6 > >();
    mutable_test< bits32, mpl::vector3_c< unsigned, 11, 11, 10 > >();
    mutable_test< bits64, mpl::vector3_c< unsigned, 21, 21, 21 > >();

    immutable_test< bits8, mpl::vector3_c< unsigned, 3, 3, 2 > >();
    immutable_test< bits16, mpl::vector3_c< unsigned, 5, 5, 6 > >();
    immutable_test< bits32, mpl::vector3_c< unsigned, 11, 11, 10 > >();
    immutable_test< bits64, mpl::vector3_c< unsigned, 21, 21, 5 > >();
}

BOOST_AUTO_TEST_SUITE_END()
