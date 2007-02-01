#pragma once

#include <boost/shared_ptr.hpp>

using namespace gil;


typedef point2<ptrdiff_t> point_t;

inline
CvPoint make_cvPoint( point_t point )
{
   return cvPoint( point.x, point.y );
}

inline
CvSize make_cvSize( point_t point )
{
   return cvSize( point.x, point.y );
}

template< class PIXEL >
struct pixel_converter
{};

template<> struct pixel_converter< gray8_pixel_t >
{
   typedef gray8_pixel_t pixel_t;

   static const unsigned int depth = IPL_DEPTH_8U;
   
   static CvScalar make_cvScalar( pixel_t pixel )
   {
      return cvScalar( pixel.gray );
   }
};


template<> struct pixel_converter< rgb8_pixel_t >
{
   typedef rgb8_pixel_t pixel_t;

   static const unsigned int depth = IPL_DEPTH_8U;
   
   static CvScalar make_cvScalar( pixel_t pixel )
   {
      return cvScalar( pixel.red, pixel.green, pixel.blue );
   }
};

template<> struct pixel_converter< bgr8_pixel_t >
{
   typedef bgr8_pixel_t pixel_t;

   static const unsigned int depth = IPL_DEPTH_8U;
   
   static CvScalar make_cvScalar( pixel_t pixel )
   {
      return cvScalar( pixel.red, pixel.green, pixel.blue );
   }
};


template< class VIEW >
class create_cv_image
{
   IplImage* _img;

   typedef typename VIEW::pixel_t pixel_t;

public:

   create_cv_image( VIEW view )
   {
      _img = cvCreateImage( make_cvSize( view.dimensions() )
                          , pixel_converter<pixel_t>::depth
                          , pixel_t::num_channels            );
      
      _img->imageData = (char*) ( &view.begin()[0] );
   }

   ~create_cv_image() { cvReleaseImage( &_img); }

   IplImage* get() { return _img; }
};

template<class VIEW>
void drawRectangle( VIEW&                  view
                  , point_t                start
                  , point_t                end
                  , typename VIEW::pixel_t pixel
                  , std::size_t            line_width )
{
   typedef VIEW::pixel_t pixel_t;

   create_cv_image<VIEW> cv_img( view );

   cvRectangle( cv_img.get()
              , make_cvPoint( start )
              , make_cvPoint( end   )
              , pixel_converter<pixel_t>::make_cvScalar( pixel )
              , line_width             );
}

template<class VIEW>
void drawCircle( VIEW&                  view
               , point_t                center
               , std::size_t            radius
               , typename VIEW::pixel_t color
               , std::size_t            line_width )
{
   typedef VIEW::pixel_t pixel_t;

   create_cv_image<VIEW> cv_img( view );

   cvCircle( cv_img.get()
           , make_cvPoint( center )
           , radius
           , pixel_converter<pixel_t>::make_cvScalar( color )
           , line_width           );
}

template<class VIEW>
void drawLine( VIEW&                  view
             , point_t                start
             , point_t                end
             , typename VIEW::pixel_t color
             , std::size_t            line_width )
{
   typedef VIEW::pixel_t pixel_t;

   create_cv_image<VIEW> cv_img( view );

   cvLine( cv_img.get()
         , make_cvPoint( start )
         , make_cvPoint( end )
         , pixel_converter<pixel_t>::make_cvScalar( color )
         , 1                                                );
}
