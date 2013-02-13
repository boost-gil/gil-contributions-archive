#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( gil_test )

//
// packed_dynamic_channel_reference< bits8, 4, true > - mutable half byte reference type
//
// In comparison to packed_channel_reference this type can set the FirstBit at run time.
//
// These tests make sure that a value is represented correctly.
//
template< typename Base, typename ChannelSizes >
void mutable_test()
{
	typedef packed_dynamic_channel_reference< typename Base
                                            , 0
                                            , true
                                            > channel1_dynamic_reference_t;

    typedef packed_dynamic_channel_reference< typename Base
                                            , mpl::at_c< ChannelSizes, 0 >::type::value 
                                                + mpl::at_c< ChannelSizes, 1 >::type::value
                                            , true
                                            > channel2_dynamic_reference_t;

	typedef packed_dynamic_channel_reference< typename Base
                                            , mpl::at_c< ChannelSizes, 1 >::type::value 
                                                + mpl::at_c< ChannelSizes, 2 >::type::value
                                            , true
                                            > channel3_dynamic_reference_t;

    // the actual value
	Base data = 0;

    // create and assign
	channel1_dynamic_reference_t channel1( &data, mpl::at_c< ChannelSizes, 0 >::type::value );
	channel2_dynamic_reference_t channel2( &data, mpl::at_c< ChannelSizes, 1 >::type::value );
    channel3_dynamic_reference_t channel3( &data, mpl::at_c< ChannelSizes, 2 >::type::value );

	channel1 = channel_traits< channel1_dynamic_reference_t >::max_value();
	channel2 = channel_traits< channel2_dynamic_reference_t >::max_value();
	channel3 = channel_traits< channel3_dynamic_reference_t >::max_value();

    BOOST_CHECK_EQUAL( data, channel_traits< Base >::max_value() );
}

template< typename Base, typename ChannelSizes >
void immutable_test()
{
	typedef packed_dynamic_channel_reference< typename Base
                                            , 0
                                            , false
                                            > channel1_dynamic_reference_t;

    typedef packed_dynamic_channel_reference< typename Base
                                            , mpl::at_c< ChannelSizes, 0 >::type::value 
                                                + mpl::at_c< ChannelSizes, 1 >::type::value
                                            , false
                                            > channel2_dynamic_reference_t;

	typedef packed_dynamic_channel_reference< typename Base
                                            , mpl::at_c< ChannelSizes, 1 >::type::value 
                                                + mpl::at_c< ChannelSizes, 2 >::type::value
                                            , false
                                            > channel3_dynamic_reference_t;

    // the actual value
	Base data = channel_traits< Base >::max_value();

    // create
	channel1_dynamic_reference_t channel1( &data, mpl::at_c< ChannelSizes, 0 >::type::value );
	channel2_dynamic_reference_t channel2( &data, mpl::at_c< ChannelSizes, 1 >::type::value );
    channel3_dynamic_reference_t channel3( &data, mpl::at_c< ChannelSizes, 2 >::type::value );

    BOOST_CHECK_EQUAL( channel1, channel_traits< channel1_dynamic_reference_t >::max_value() );
    BOOST_CHECK_EQUAL( channel2, channel_traits< channel2_dynamic_reference_t >::max_value() );
    BOOST_CHECK_EQUAL( channel3, channel_traits< channel3_dynamic_reference_t >::max_value() );
}

BOOST_AUTO_TEST_CASE( packed_dynamic_channel_reference_test )
{
    // use BOOST_PP_REPEAT_FROM_TO for complete test
    mutable_test< bits8, mpl::vector3_c< unsigned, 3, 3, 2 > >();
    mutable_test< bits16, mpl::vector3_c< unsigned, 5, 5, 6 > >();
    mutable_test< bits32, mpl::vector3_c< unsigned, 11, 11, 10 > >();
    mutable_test< bits64, mpl::vector3_c< unsigned, 21, 21, 22 > >();

    immutable_test< bits8, mpl::vector3_c< unsigned, 3, 3, 2 > >();
    immutable_test< bits16, mpl::vector3_c< unsigned, 5, 5, 6 > >();
    immutable_test< bits32, mpl::vector3_c< unsigned, 11, 11, 10 > >();
    immutable_test< bits64, mpl::vector3_c< unsigned, 21, 21, 22 > >();
}

BOOST_AUTO_TEST_SUITE_END()
