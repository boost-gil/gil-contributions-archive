#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;
using namespace chrono;
using namespace gil;


milliseconds last_time;
milliseconds cumulative_time;

int FPS = 60;
float frame_length_ms = 1000.f / FPS;

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

        //this_thread::sleep( boost::posix_time::milliseconds( 10 ));
    }
}

void update_frame()
{
    int counter = 0;

    typedef time_point<steady_clock> tp_t;
    tp_t last_time = steady_clock::now();

    float cumulative = 0.f;

    while( true )
    {
        tp_t current = steady_clock::now();
        
        milliseconds delta = duration_cast<milliseconds> (steady_clock::now() - last_time);

        if(delta.count() > frame_length_ms )
        {
            {
                boost::lock_guard< boost::mutex > l( render_object );

                copy_pixels( view( img ), view(display));
            }

            last_time = current;
            
            if( counter % 100 == 0 )
            {
                cout << 1000.f / delta.count() << "fps" << endl;

                counter = 0;
            }
            else
            {
                ++counter;
            }
        }
        else
        {
            //this_thread::sleep( boost::posix_time::milliseconds( 10 ));
        }
    }
}


BOOST_AUTO_TEST_CASE( test_sync )
{
    cout << "FPS: " << FPS << endl;
    cout << "frame length: " << frame_length_ms << "ms" << endl;

    boost::thread render_thread( render_something );
    boost::thread display_thread( update_frame );

    render_thread.join();
    display_thread.join();


    update_frame();


    cout << "finished" << endl;
}
