/*
    Copyright 2013 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#include <SDL.h>

#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/sdl2/service.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

using namespace std;
using namespace boost;
using namespace gil;
using namespace sdl;


struct walker_redraw_handler
{
    template< typename View >
    void operator() ( View v )
    {
        typedef typename View::value_type pixel_t;

        static bool refill = true;

        if( refill )
        {
            fill_pixels( v, pixel_t( 0, 0, 0, 0 ));
            refill = false;
        }

        static int x = v.dimensions().x / 2;
        static int y = v.dimensions().y / 2;

        x += ( std::rand() % 3 ) - 1;
        y += ( std::rand() % 3 ) - 1;

        *v.xy_at( x, y ) = pixel_t( 255, 255, 255, 255 );
    }
};

struct random_number_distribution_redraw_handler
{
    random_number_distribution_redraw_handler()
    : num_buckets(20)
    , buckets(num_buckets)
    {
        fill( buckets.begin(), buckets.end(), 0 );
    }

    ~random_number_distribution_redraw_handler()
    {
    }

    template< typename View >
    void operator() ( View v )
    {
        typedef typename View::value_type pixel_t;

        static bool refill = true;

        if( refill )
        {
            fill_pixels( v, pixel_t( 0, 0, 0, 0 ));
            refill = false;
        }

        buckets[ std::rand() % num_buckets ]++;
        
        
        auto bucket_width = 
            ( v.dimensions().x % num_buckets == 0 )
            ? ( v.dimensions().x ) / num_buckets
            : ( v.dimensions().x - 1 ) / num_buckets;

        auto up_down_view = flipped_up_down_view(v);

        for( int i = 0; i < num_buckets; ++i )
        {
            //cout << i * bucket_width << " " 
            //     << 0 << " "
            //     << ( i  + 1 )* bucket_width << " "
            //     << bucketss[i]
            //     << endl;


            fill_pixels( subimage_view( up_down_view
                                      , i * bucket_width
                                      , 0
                                      , bucket_width
                                      , buckets[i]
                                      ), pixel_t( 255, 255, 0, 255 ));
        }
    }

    int num_buckets;
    vector<int> buckets;
};


struct keyboard_event_handler
{
    template< typename Window >
    void operator() ( Window& w
                    , event_t e
                    )
    {
        w.set_cancel( true );
    }
};

BOOST_AUTO_TEST_CASE( test_sdl_basic )
{
    service sdl_service;

    window< random_number_distribution_redraw_handler, keyboard_event_handler > w(30);
    sdl_service.add_window( w );

    sdl_service.run();
}
