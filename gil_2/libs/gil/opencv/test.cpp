// drawing.cpp : Defines the entry point for the console application.
//

#include <iomanip>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io/bmp_io.hpp>

#include <boost/gil/extension/opencv/drawing.hpp>
#include <boost/gil/extension/opencv/resize.hpp>
#include <boost/gil/extension/opencv/drawing.hpp>
#include <boost/gil/extension/opencv/resize.hpp>
#include <boost/gil/extension/opencv/edge_detection.hpp>
#include <boost/gil/extension/opencv/smoothing.hpp>

using namespace boost;
using namespace gil;
using namespace opencv;

int main(int argc, char* argv[] )
{
   typedef point2<ptrdiff_t> point_t;

/// rectangle
   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );
      drawRectangle( ipl_img
                   , point_t( 100, 100 )
                   , point_t( 200, 200 )
                   , rgb8_pixel_t( 255, 0, 0 )
                   , 1                          );

      bmp_write_view( ".\\rectangle_test.bmp", view( img ) );
   }

   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawRectangle( ipl_img
                   , point_t( 100, 100 )
                   , point_t( 200, 200 )
                   , rgb8_pixel_t( 255, 0, 0 )
                   , 1                          );

      drawRectangle( ipl_img
                   , point_t( 200, 200 )
                   , point_t( 300, 300 )
                   , rgb8_pixel_t( 255, 0, 0 )
                   , 1                          );


      bmp_write_view( ".\\rectangle_test_2.bmp", view( img ) );
   }

/// circle
   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawCircle( ipl_img
                , point_t( 100, 100 )
                , 100
                , rgb8_pixel_t( 255, 0, 0 )
                , 1                          );

      bmp_write_view( ".\\circle_test.bmp", view( img ) );
   }

   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawCircle( ipl_img
                , point_t( 100, 100 )
                , 100
                , rgb8_pixel_t( 255, 0, 0 )
                , 1                          );

      drawCircle( ipl_img
                , point_t( 200, 200 )
                , 100
                , rgb8_pixel_t( 255, 0, 0 )
                , 1                          );


      bmp_write_view( ".\\cricle_test_2.bmp", view( img ) );
   }

/// line
   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawLine( ipl_img
              , point_t( 100, 100 )
              , point_t( 200, 200 )
              , rgb8_pixel_t( 255, 0, 0 )
              , 1
              , anti_aliased_Line() );

      bmp_write_view( ".\\line_test.bmp", view( img ) );
   }

   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawLine( ipl_img
              , point_t( 100, 100 )
              , point_t( 200, 200 )
              , rgb8_pixel_t( 255, 0, 0 )
              , 1
              , eight_connected_Line()    );

      drawLine( ipl_img
              , point_t( 250, 250 )
              , point_t( 350, 350 )
              , rgb8_pixel_t( 255, 0, 0 )
              , 1 
              , four_connected_Line()     );

      bmp_write_view( ".\\line_test_2.bmp", view( img ) );
   }

/// polyline
   {
      rgb8_image_t img( 640, 480 );
      fill_pixels( view(img), rgb8_pixel_t( 0, 0, 0 ));

      curve_t curve1;
      curve1.push_back( point_t( 10,10 ));
      curve1.push_back( point_t( 10,100 ));
      curve1.push_back( point_t( 100,100 ));
      curve1.push_back( point_t( 100,10 ));

      curve_t curve2;
      curve2.push_back( point_t( 30,30 ));
      curve2.push_back( point_t( 30,130 ));
      curve2.push_back( point_t( 130,130 ));
      curve2.push_back( point_t( 130,30 ));
      curve2.push_back( point_t( 150,10 ));

      curve_vec_t cur_vec;
      cur_vec.push_back( curve1 );
      cur_vec.push_back( curve2 );

      ipl_image_wrapper ipl_img = create_ipl_image( view( img ) );

      drawPolyLine( ipl_img
                  , cur_vec
                  , true
                  , rgb8_pixel_t( 255, 0, 0 )
                  , 1                          );

      bmp_write_view( ".\\polyline_test.bmp", view( img ) );
   }

   {
      rgb8_image_t in;
      bmp_read_image( ".\\rgb.bmp", in );
      ipl_image_wrapper ipl_in = create_ipl_image( view( in ) );

      rgb8_image_t target( 320, 240 );
      ipl_image_wrapper ipl_target = create_ipl_image( view( target ) );

      opencv::resize( ipl_in, ipl_target, nearest_neigbor() );
      bmp_write_view( ".\\nn_rgb_resized.bmp",  view( target ));

      opencv::resize( ipl_in, ipl_target, bilinear() );
      bmp_write_view( ".\\bilinear_rgb_resized.bmp",  view( target ));

      opencv::resize( ipl_in, ipl_target, area() );
      bmp_write_view( ".\\area_rgb_resized.bmp",  view( target ));

      opencv::resize( ipl_in, ipl_target, bicubic() );
      bmp_write_view( ".\\bicubic_rgb_resized.bmp",  view( target ));
   }

// edge detection

   // sobel
   {
      rgb8_image_t in;
      bmp_read_image( ".\\rgb.bmp", in );
      ipl_image_wrapper ipl_in = create_ipl_image( view( in ) );

      rgb8_image_t out( in.dimensions() );
      ipl_image_wrapper ipl_out = create_ipl_image( view( out ) );

      sobel( ipl_in, ipl_out );

      bmp_write_view( ".\\sobel.bmp",  view( out ));
   }

   // laplace
   {
      rgb8_image_t in;
      bmp_read_image( ".\\rgb.bmp", in );
      ipl_image_wrapper ipl_in = create_ipl_image( view( in ) );

      rgb16s_image_t temp( in.dimensions() );
      ipl_image_wrapper ipl_temp = create_ipl_image( view( temp ));

      laplace( ipl_in
             , ipl_temp );

      rgb8_image_t out( in.dimensions() );
      ipl_image_wrapper ipl_out = create_ipl_image( view( out ) );

      cvConvertScaleAbs( ipl_temp.get()
                       , ipl_out.get()
                       , 1, 0 );

      bmp_write_view( ".\\laplace.bmp",  view( out ));
   }

}
