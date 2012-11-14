#include <boost/test/unit_test.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/gil/extension/toolbox/metafunctions/channel_view.hpp>

using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( channel_view_test )

BOOST_AUTO_TEST_CASE( channel_view_test )
{
    rgb8_image_t img( 100, 100 );
    //typedef typename kth_channel_view_type< 0, const rgb8_view_t>::type view_t;
    auto red = kth_channel_view<0>( const_view( img ));

    //typedef channel_view_type< red_t, const rgb8_view_t>::type view_t;
    auto red_ = channel_view< red_t >( const_view( img ));

    BOOST_STATIC_ASSERT(( is_same< kth_channel_view_type< 0, const rgb8_view_t>::type
                                 , channel_view_type< red_t, const rgb8_view_t>::type
                                 >::value
                       ));
}

BOOST_AUTO_TEST_SUITE_END()
