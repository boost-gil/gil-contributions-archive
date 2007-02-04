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
inline
CvScalar make_cvScalar( const PIXEL& pixel )
{
   CvScalar s;
   for( int c=0; c<PIXEL::num_channels; ++c )
   {
      s.val[c] = pixel[c];
   }

   return s;
}


// function that provides gil to IPL_DEPTH_<bit_depth>(S|U|F) mapping
template< class VIEW >
inline
unsigned int get_depth( const VIEW& view )
{
   typedef VIEW::channel_t channel_t;

   switch( sizeof( channel_t ))
   {
      case 1:
      {
         // either IPL_DEPTH_8U or IPL_DEPTH_8S
         return IPL_DEPTH_8U;
      }

      case 2:
      {
         // either IPL_DEPTH_16U or IPL_DEPTH_16S
         return IPL_DEPTH_16U;
      }

      case 3:
      {
         // either IPL_DEPTH_32S or IPL_DEPTH_32F
         return IPL_DEPTH_32S;
      }

      case 4:
      {
         // only IPL_DEPTH_64F
         return IPL_DEPTH_64F;
      }

      default:
      {
         assert( false );
         return 0;
      }
   }
}



template< class VIEW >
class create_cv_image
{
   IplImage* _img;


public:

   create_cv_image( VIEW view )
   {
      const unsigned int num_channels = VIEW::num_channels;

      _img = cvCreateImage( make_cvSize( view.dimensions() )
                          , get_depth( view )
                          , num_channels                       );
      
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
   create_cv_image<VIEW> cv_img( view );

   cvRectangle( cv_img.get()
              , make_cvPoint( start )
              , make_cvPoint( end   )
              , make_cvScalar( pixel )
              , line_width             );
}

template<class VIEW>
void drawCircle( VIEW&                  view
               , point_t                center
               , std::size_t            radius
               , typename VIEW::pixel_t color
               , std::size_t            line_width )
{
   create_cv_image<VIEW> cv_img( view );

   cvCircle( cv_img.get()
           , make_cvPoint( center )
           , radius
           , make_cvScalar( color )
           , line_width             );
}

template<class VIEW>
void drawLine( VIEW&                  view
             , point_t                start
             , point_t                end
             , typename VIEW::pixel_t color
             , std::size_t            line_width )
{
   create_cv_image<VIEW> cv_img( view );

   cvLine( cv_img.get()
         , make_cvPoint( start )
         , make_cvPoint( end )
         , make_cvScalar( color )
         , line_width             );
}
