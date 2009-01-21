#include "stdafx.h"

#include <cassert>

#include <boost/mpl/for_each.hpp>
#include <boost/gil/extension/io_new/tiff_read.hpp>

using namespace std;
using namespace boost;
using namespace gil;

struct compare_channel_sizes_fn
{
    compare_channel_sizes_fn( unsigned int* a )
    : _a( a )
    , _b( true )
    {}

    template< typename ChannelSize >
    void operator()( ChannelSize x)
    {
        if( x != *_a++ )
        {
            _b = false;
        }
    }

    unsigned int* _a;
    bool _b;
};

template< typename T >
struct channel_sizes_type {};

template< typename B, typename C, typename L, bool M >
struct channel_sizes_type< bit_aligned_pixel_reference< B, C, L, M > > { typedef C type; };

template< typename B, typename C, typename L, bool M >
struct channel_sizes_type< const bit_aligned_pixel_reference< B, C, L, M > > { typedef C type; };

int main()
{
    typedef bit_aligned_image3_type< 5, 6, 5, rgb_layout_t >::type rgb565_image_t;
    typedef rgb565_image_t::view_t::reference reference_t;

    std::vector<unsigned int> channel_sizes;
    channel_sizes.push_back( 5 );
    channel_sizes.push_back( 6 );
    channel_sizes.push_back( 5 );

    typedef channel_sizes_type< reference_t >::type cs_t;

    compare_channel_sizes_fn fn( &channel_sizes.front() );
    mpl::for_each< cs_t >( fn );

    assert( fn._b );
}
