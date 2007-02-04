#pragma once

#include <vector>

#include <boost/scoped_array.hpp>

using namespace gil;


typedef point2<ptrdiff_t> point_t;
typedef std::vector< point_t > curve_t;
typedef std::vector< curve_t > curve_vec_t;

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
   for( int i = 0; i < PIXEL::num_channels; ++i )
   {
      s.val[i] = pixel[i];
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

template<class VIEW>
void drawPolyLine( VIEW&                  view
                 , const curve_vec_t&     curves
                 , bool                   is_closed
                 , typename VIEW::pixel_t color
                 , std::size_t            line_width )
{
   create_cv_image<VIEW> cv_img( view );

   const std::size_t num_curves = curves.size();

   boost::scoped_array<std::size_t> num_points_per_curve( new std::size_t[num_curves] );

   std::size_t total_num_points = 0;
   for( std::size_t i = 0; i < num_curves; ++i )
   {
      std::size_t num_points = curves[i].size();

      num_points_per_curve[i] = num_points;
      total_num_points += num_points;
   }

/* doesnn't work like this. needs to be a CvPoint**
   boost::scoped_array<CvPoint> all_points( new CvPoint[ total_num_points ] );
   for( std::size_t j = 0; j < num_curves; ++j )
   {
      const std::vector<point_t>& points_vec = curves[j];

      for( std::size_t i = 0; i < total_num_points; ++i )
      {
         all_points[i] = make_cvPoint( points_vec[i] );
      }
   }



   cvPolyLine( cv_img.get()
             , all_points.get()  // needs to be pointer to C array of CvPoints.
             , num_points_per_curve.get() // int array that contains number of points of each curve.
             , num_curves
             , is_closed
             , make_cvScalar( color )
             , line_width             );

*/
}
