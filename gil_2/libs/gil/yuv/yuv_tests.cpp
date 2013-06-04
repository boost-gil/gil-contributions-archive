#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/toolbox/color_spaces/ycbcr.hpp>

#include "yuv_step_iterator.hpp"

namespace boost {
namespace gil {

template <typename T, typename Cs>  // mutable   planar
struct iterator_is_basic<yuv_planar_pixel_iterator<      T*,Cs> > : public mpl::true_ {};
template <typename T, typename Cs>    // immutable planar
struct iterator_is_basic<yuv_planar_pixel_iterator<const T*,Cs> > : public mpl::true_ {};

template <typename T, typename Cs>  // mutable   planar step
struct iterator_is_basic<memory_based_step_iterator<yuv_planar_pixel_iterator<      T*,Cs> > > : public mpl::true_ {};
template <typename T, typename Cs>    // immutable planar step
struct iterator_is_basic<memory_based_step_iterator<yuv_planar_pixel_iterator<const T*,Cs> > > : public mpl::true_ {};



/// \ingroup ImageViewConstructors
/// \brief from raw YUV planar data
template <typename ChannelPtr>
inline
typename type_from_x_iterator< yuv_planar_pixel_iterator< ChannelPtr
                                                        , ycbcr_601__t
                                                        >
                             >::view_t
planar_yuv_view( std::size_t width
               , std::size_t height
               , ChannelPtr y
               , ChannelPtr v
               , ChannelPtr u
               , std::ptrdiff_t rowsize_in_bytes
               )
{
    typedef typename type_from_x_iterator< yuv_planar_pixel_iterator< ChannelPtr
                                                                    , ycbcr_601__t
                                                                    >
                                         >::view_t RView;

    return RView( width
                , height
                , typename RView::locator( yuv_planar_pixel_iterator< ChannelPtr
                                                                    , ycbcr_601__t
                                                                    >( y, v, u )
                                         , rowsize_in_bytes
                                         )
                );
}

} // namespace gil
} // namespace boost

using namespace std;
using namespace boost;
using namespace gil;

BOOST_AUTO_TEST_SUITE( gil_test )

BOOST_AUTO_TEST_CASE( x_iterator_test )
{
    typedef unsigned char* channel_ptr_t;
    typedef mpl::vector3<red_t,green_t,blue_t> color_space_t;
    typedef yuv_planar_pixel_iterator< channel_ptr_t, color_space_t > x_iterator;

    // three planes
    // image: 640 x 640
    int N = 640;
    vector< unsigned char > image( N * N + N * N / 2  );

    channel_ptr_t y_base = &image.front();
    channel_ptr_t v_base = y_base + ( N * N );
    channel_ptr_t u_base = v_base + N;

    x_iterator it( y_base, v_base, u_base );
    it++;

    assert( at_c<0>( it ) == y_base + 1 );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it--;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it += 7;

    assert( at_c<0>( it ) == y_base + 7 );
    assert( at_c<1>( it ) == v_base + 3 );
    assert( at_c<2>( it ) == u_base + 3 );

    it -= 7;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it += 8;

    assert( at_c<0>( it ) == y_base + 8 );
    assert( at_c<1>( it ) == v_base + 4 );
    assert( at_c<2>( it ) == u_base + 4 );

    it -= 8;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );
}

BOOST_AUTO_TEST_CASE( view_test )
{
    typedef unsigned char* channel_ptr_t;
    typedef ycbcr_601__t color_space_t;
    typedef yuv_planar_pixel_iterator< channel_ptr_t, color_space_t > x_iterator;

    // three planes
    // image: 640 x 640
    int N = 640;
    vector< unsigned char > image( N * N + N * N / 2  );

    channel_ptr_t y_base = &image.front();
    channel_ptr_t v_base = y_base + ( N * N );
    channel_ptr_t u_base = v_base + N;


    typedef type_from_x_iterator< yuv_planar_pixel_iterator< channel_ptr_t
                                                           , ycbcr_601__t
                                                           >
                                >::view_t view_t;

    view_t v = planar_yuv_view<channel_ptr_t >( N, N, y_base, v_base, u_base, N );

    auto raw_plane_0 = planar_view_get_raw_data( v, 0 );
    auto raw_plane_1 = planar_view_get_raw_data( v, 1 );
    auto raw_plane_2 = planar_view_get_raw_data( v, 2 );

    assert( y_base == raw_plane_0 );
    assert( v_base == raw_plane_1 );
    assert( u_base == raw_plane_2 );

    typedef kth_channel_view_type< 0, view_t >::type plane_t;
    plane_t plane_view_0 = kth_channel_view<0>( v );
    plane_t plane_view_1 = kth_channel_view<1>( v );
    plane_t plane_view_2 = kth_channel_view<2>( v );

    //typedef view_t::value_type pixel_t;
    //pixel_t p = pixel_t( 200, 100, 50 );

    //fill_pixels(v, p);

}

BOOST_AUTO_TEST_SUITE_END()