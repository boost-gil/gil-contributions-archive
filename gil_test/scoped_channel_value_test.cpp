#include "stdafx.h"

#include <boost/gil/gil_all.hpp>
#include <boost/test/unit_test.hpp>

#include "base_types.hpp"

namespace boost { namespace gil {

template< int K > 
struct min_value
{
    typedef typename detail::min_fast_uint< K >::type integer_t;

    static integer_t apply() { return 0; }
};

template< int K > 
struct max_value
{
    typedef typename detail::min_fast_uint< K >::type integer_t;

    ///@todo why not unsigned_integral_max_value< K > ?
    static integer_t apply() { return detail::unsigned_integral_max_value< packed_channel_value< K > >::value; }
};

} // namespace gil
} // namespace boost

template< int K >
struct make_scoped_channel_value
{
    typedef typename boost::gil::detail::min_fast_uint< K >::type integer_t;

    typedef boost::gil::scoped_channel_value< integer_t, boost::gil::min_value< K >, boost::gil::max_value< K > > type;
};

using namespace boost::gil;

BOOST_AUTO_TEST_SUITE( gil_test )

BOOST_AUTO_TEST_CASE( scoped_channel_test )
{
	typedef make_scoped_channel_value< 12 >::integer_t integer_t;
    typedef make_scoped_channel_value< 12 >::type bits12_t;

    integer_t max =  channel_traits< bits12_t >::max_value();
    BOOST_CHECK_EQUAL( channel_convert<integer_t>(max), max );
}

BOOST_AUTO_TEST_SUITE_END()
