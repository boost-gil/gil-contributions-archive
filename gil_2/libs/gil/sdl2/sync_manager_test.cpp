/*
    Copyright 2013 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;
using namespace chrono;
using namespace gil;

// http://www.koonsolo.com/news/dewitters-gameloop/

milliseconds last_time;
milliseconds cumulative_time;

int FPS = 60;
float frame_length_ms = 1000.f / FPS;
int skip_ticks = 1000 / FPS;

boost::gil::rgb8_image_t img( 640, 480 );
boost::gil::rgb8_image_t display( 640, 480 );

boost::mutex render_object;

void render_something()
{
    while(true)
    {
        {
            boost::lock_guard< boost::mutex > l( render_object );

            fill_pixels( view( img ), rgb8_pixel_t( 255, 0, 0 ));
        }

        this_thread::sleep( boost::posix_time::milliseconds( 100 ));
    }
}

void update_frame()
{
    int counter = 0;

    typedef time_point<steady_clock> tp_t;

    tp_t last_time = steady_clock::now();
    milliseconds next_tick = duration_cast<milliseconds>( last_time.time_since_epoch() );

    while( true )
    {
        {
            //boost::lock_guard< boost::mutex > l( render_object );

            //copy_pixels( view( img ), view(display));

            //tp_t current = steady_clock::now();
            //cout << 1000 / duration_cast<milliseconds>( current - last_time ).count() << endl;
            
            
            tp_t current = steady_clock::now();
            static unsigned int counter = 0;
            if( ++counter % 100 == 0 )
            {
                milliseconds diff = duration_cast<milliseconds>( current - last_time );
                if( diff.count() > 0 )
                {
                    cout << 1000 / diff.count() << endl;
                }
            
                counter = 0;
            }

            last_time = current;
        }

        // skip_ticks for 60fps is 16
        next_tick += milliseconds( skip_ticks );
        milliseconds sleep_time = duration_cast<milliseconds>( next_tick - steady_clock::now().time_since_epoch() );

        if( sleep_time.count() > 0 )
        {
            this_thread::sleep( boost::posix_time::milliseconds( sleep_time.count() ));
        }
        else
        {
            // we are running behind
        }
    }
}


BOOST_AUTO_TEST_CASE( test_sync )
{
    cout << "FPS: " << FPS << endl;
    cout << "frame length: " << frame_length_ms << "ms" << endl;

    //boost::thread render_thread( render_something );
    boost::thread display_thread( update_frame );

    //render_thread.join();
    display_thread.join();

    cout << "finished" << endl;
}
