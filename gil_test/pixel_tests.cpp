#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>


namespace boost { namespace gil {
	//////////////////////////////////////////////////////////////////////////
	// Byte-aligned 12 bit grayscale
	//////////////////////////////////////////////////////////////////////////
	const uint16_t bits12_max_val = 0xFFF;
	struct bits12_min { static uint16_t apply() { return 0; } };
	struct bits12_max { static uint16_t apply() { return bits12_max_val; } };

	typedef scoped_channel_value<uint16_t, bits12_min, bits12_max> bits12;

	namespace detail
	{
		template<> struct unsigned_integral_max_value< bits12 > : public mpl::integral_c<uint32_t, bits12_max_val> {};
		template<> struct unsigned_integral_num_bits< bits12 >  : public mpl::int_<12> {};

	}//namespace detail

	GIL_DEFINE_BASE_TYPEDEFS(12, gray)
}// namespace gil
}// namespace boost

using namespace boost::gil;



BOOST_AUTO_TEST_SUITE( gil_test )
	
BOOST_AUTO_TEST_CASE( pixel_test )
{
	gray12_pixel_t g14(100);
	rgb8_pixel_t rgb8;

	color_convert(g14, rgb8);
	color_convert(rgb8, g14);
}

BOOST_AUTO_TEST_SUITE_END()