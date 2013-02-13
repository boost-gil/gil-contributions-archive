#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( gil_test )

struct double_zero { static double apply() { return 0.0; } };
struct double_one  { static double apply() { return 1.0; } };

BOOST_AUTO_TEST_CASE( channel_test )
{
    typedef uint64_t bits64;
    typedef int64_t bits64s;

    typedef scoped_channel_value<double, double_zero, double_one> bits64f;

    bits8   c8_min   =  channel_traits<bits8  >::min_value();
    bits8   c8_max   =  channel_traits<bits8  >::max_value();
    bits8s  c8s_min  =  channel_traits<bits8s >::min_value();
    bits8s  c8s_max  =  channel_traits<bits8s >::max_value();
    bits16  c16_min  =  channel_traits<bits16 >::min_value();
    bits16  c16_max  =  channel_traits<bits16 >::max_value();
    bits16s c16s_min =  channel_traits<bits16s>::min_value();
    bits16s c16s_max =  channel_traits<bits16s>::max_value();
    bits32  c32_min  =  channel_traits<bits32 >::min_value();
    bits32  c32_max  =  channel_traits<bits32 >::max_value();
    bits32s c32s_min =  channel_traits<bits32s>::min_value();
    bits32s c32s_max =  channel_traits<bits32s>::max_value();
    bits32f c32f_min =  channel_traits<bits32f>::min_value();
    bits32f c32f_max =  channel_traits<bits32f>::max_value();
    bits64  c64_min  =  channel_traits<bits64 >::min_value();
    bits64  c64_max  =  channel_traits<bits64 >::max_value();
    bits64s c64s_min =  channel_traits<bits64s>::min_value();
    bits64s c64s_max =  channel_traits<bits64s>::max_value();
    bits64f c64f_min =  channel_traits<bits64f>::min_value();
    bits64f c64f_max =  channel_traits<bits64f>::max_value();

    // this generates a VS compiler warning
    //boost::mpl::integral_c<bits64,-1> kk;


    //value test - channel_convert
    BOOST_CHECK_EQUAL( channel_convert<bits8>(c8_max), c8_max );
    BOOST_CHECK_EQUAL( channel_convert<bits16>(c8_max), c16_max );
    BOOST_CHECK_EQUAL( channel_convert<bits32>(c8_max), c32_max );
    //BOOST_CHECK_EQUAL( channel_convert<bits64>(c8_max), c64_max );

    //reference test

    // packed reference test
    typedef packed_channel_reference<boost::uint16_t, 0,5,true> channel16_0_5_reference_t;
    typedef packed_channel_reference<boost::uint16_t, 5,6,true> channel16_5_6_reference_t;
    typedef packed_channel_reference<boost::uint16_t, 11,5,true> channel16_11_5_reference_t;

    bits16 data = 0;
    channel16_0_5_reference_t   channel1(&data);
    channel16_5_6_reference_t   channel2(&data);
    channel16_11_5_reference_t  channel3(&data);

    channel1=channel_traits<channel16_0_5_reference_t>::max_value();
    channel2=channel_traits<channel16_5_6_reference_t>::max_value();
    channel3=channel_traits<channel16_11_5_reference_t>::max_value();
    BOOST_CHECK_EQUAL( data, 65535 );
    
}

BOOST_AUTO_TEST_SUITE_END()
