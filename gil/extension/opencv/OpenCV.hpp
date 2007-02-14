#pragma once

#include <vector>

#include <boost/shared_array.hpp>

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>


using namespace gil;

typedef point2<ptrdiff_t> point_t;
typedef std::vector< point_t > curve_t;
typedef std::vector< curve_t > curve_vec_t;

typedef boost::shared_array<CvPoint> cvpoint_array_t;
typedef std::vector< cvpoint_array_t > cvpoint_array_vec_t;

inline
CvPoint make_cvPoint( point_t point )
{
   return cvPoint( point.x, point.y );
}

inline
cvpoint_array_t make_cvPoint_array( const curve_t& curve )
{
   std::size_t curve_size = curve.size();

   cvpoint_array_t cvpoint_array( new CvPoint[ curve.size() ] );

   for( std::size_t i = 0; i < curve_size ; ++i )
   {
      cvpoint_array[i] = make_cvPoint( curve[i] );
   }

   return cvpoint_array;
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

// taken from Hirotaka's stllcv code
typedef boost::mpl::map<
	 boost::mpl::pair<gil::bits8	,  boost::mpl::int_<IPL_DEPTH_8U> >
,	 boost::mpl::pair<gil::bits16	,  boost::mpl::int_<IPL_DEPTH_16U> >
,	 boost::mpl::pair<gil::bits32f	,  boost::mpl::int_<IPL_DEPTH_32F> >
,	 boost::mpl::pair<gil::bits64f	,  boost::mpl::int_<IPL_DEPTH_64F> >
,	 boost::mpl::pair<gil::bits8s	,  boost::mpl::int_<IPL_DEPTH_8S> >
,	 boost::mpl::pair<gil::bits16s	,  boost::mpl::int_<IPL_DEPTH_16S> >
,	 boost::mpl::pair<gil::bits32s	,  boost::mpl::int_<IPL_DEPTH_32S> >
    > ipl_depth_map_from_channel_t_map;


typedef boost::mpl::map<
	 boost::mpl::pair<gil::gray_t	,  boost::mpl::int_<1> >
,	 boost::mpl::pair<gil::bgr_t	,  boost::mpl::int_<3> >
,	 boost::mpl::pair<gil::bgra_t	,  boost::mpl::int_<4> >
,	 boost::mpl::pair<gil::rgb_t	,  boost::mpl::int_<3> >
,	 boost::mpl::pair<gil::rgba_t	,  boost::mpl::int_<4> >
    > ipl_nchannels_from_gil_color_space_map;

template< class VIEW >
class create_cv_image
{
   IplImage* _img;

   typedef typename VIEW::channel_t channel_t;
   typedef typename VIEW::color_space_t color_space_t;

public:

   create_cv_image( VIEW view )
   {
      _img = cvCreateImage( make_cvSize( view.dimensions() )
			                 , boost::mpl::at< ipl_depth_map_from_channel_t_map		, channel_t>::type::value
			                 , boost::mpl::at< ipl_nchannels_from_gil_color_space_map	, color_space_t>::type::value );

      if( !_img )
      {
         throw std::runtime_error( "Cannot create IPL image." );
      }

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

   boost::scoped_array<int> num_points_per_curve( new int[num_curves] );

   std::size_t total_num_points = 0;
   for( std::size_t i = 0; i < num_curves; ++i )
   {
      num_points_per_curve[i] = curves[i].size();
   }

   // The curve array vector will deallocate all memory by itself.
   cvpoint_array_vec_t pp( num_curves );

   CvPoint** curve_array = new CvPoint*[num_curves];

   for( std::size_t i = 0; i < num_curves; ++i )
   {
      pp[i] = make_cvPoint_array( curves[i] );

      curve_array[i] = pp[i].get();
   }
   
   cvPolyLine( cv_img.get()
             , curve_array  // needs to be pointer to C array of CvPoints.
             , num_points_per_curve.get()// int array that contains number of points of each curve.
             , curves.size()
             , is_closed
             , make_cvScalar( color )
             , line_width             );

}


template<class VIEW>
void drawFillPoly( VIEW&                  view
                 , const curve_vec_t&     curves
                 , typename VIEW::pixel_t color   )
{
   create_cv_image<VIEW> cv_img( view );

   const std::size_t num_curves = curves.size();

   boost::scoped_array<int> num_points_per_curve( new int[num_curves] );

   std::size_t total_num_points = 0;
   for( std::size_t i = 0; i < num_curves; ++i )
   {
      num_points_per_curve[i] = curves[i].size();
   }

   // The curve array vector will deallocate all memory by itself.
   cvpoint_array_vec_t pp( num_curves );

   CvPoint** curve_array = new CvPoint*[num_curves];

   for( std::size_t i = 0; i < num_curves; ++i )
   {
      pp[i] = make_cvPoint_array( curves[i] );

      curve_array[i] = pp[i].get();
   }
   
   cvFillPoly( cv_img.get()
             , curve_array                // needs to be pointer to C array of CvPoints.
             , num_points_per_curve.get() // int array that contains number of points of each curve.
             , curves.size()
             , make_cvScalar( color ));
}

struct cv_font
{
   double hScale;
   double vScale;

   // e.g. CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC
   int font_description;

   std::size_t line_width;
};

template<class VIEW>
void putText( VIEW&                  view
            , const cv_font&         font
            , const std::string&     text
            , const poin_t&          start_point
            , typename VIEW::pixel_t color       )
{

   CvFont font = make_cv_font( cvFont )
}