
#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/bmp_write.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/gil/extension/sdl2/service.hpp>
#include <boost/gil/extension/sdl2/window.hpp>

using namespace std;
using namespace boost;
using namespace gil;
using namespace cv;
using namespace sdl;

struct redraw_handler
{
    redraw_handler()
    {
        cv_img = Mat( 480, 640, CV_8UC3 );
        cv_img.setTo(Scalar( 255, 255, 0 ));
    }

    template< typename View >
    void operator() ( View v )
    {
        circle( cv_img, Point( 320, 240 ), 20, Scalar( 255, 0, 0 ));
        
        unsigned char* data = interleaved_view_get_raw_data( v );
        Mat continuousRGBA(cv_img.size(), CV_8UC4, data);
        cvtColor(cv_img, continuousRGBA, CV_BGR2RGBA, 4);

        //Mat cv_img( 480, 640, CV_8UC3, data );
        //cv_img.setTo(Scalar( 255, 0, 0 ));
        //circle( cv_img, Point(320, 240 ), 20, Scalar(255, 0, 0, 255 ));
    }


    Mat cv_img;
};

BOOST_AUTO_TEST_CASE( test_opencv_basic )
{
    //Mat cv_img( 480, 640, CV_8UC3 );
    //cv_img.setTo(Scalar( 255, 255, 0 ));
    //circle( cv_img, Point( 320, 240 ), 20, Scalar( 255, 0, 0 ));

    //imwrite( "./test.bmp", cv_img );

    //rgba8_image_t img( 640, 480 );
    //unsigned char* data = interleaved_view_get_raw_data( view( img ));
    //
    //Mat continuousRGBA(cv_img.size(), CV_8UC4, data);
    //cvtColor(cv_img, continuousRGBA, CV_BGR2RGBA, 4);

    //write_view( "./gil_image.bmp", view( img ), bmp_tag() );

    service sdl_service;

    window<redraw_handler> w;
    sdl_service.add_window( w );

    sdl_service.run();
}
