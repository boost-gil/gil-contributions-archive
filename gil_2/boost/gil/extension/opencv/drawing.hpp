#ifndef DRAWING_HPP
#define DRAWING_HPP

#include "ipl_image_wrapper.hpp"

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

namespace boost { namespace gil {
namespace opencv {

/// rectangle

template< class PIXEL >
void drawRectangle( ipl_image_wrapper&  ipl_image
                  , point_t             start
                  , point_t             end
                  , PIXEL               pixel
                  , std::size_t         line_width )
{
   cvRectangle( ipl_image.get()
              , make_cvPoint( start )
              , make_cvPoint( end   )
              , make_cvScalar( pixel )
              , static_cast<int>( line_width ));
}

template<class VIEW>
void drawRectangle( VIEW&                  view
                  , point_t                start
                  , point_t                end
                  , typename VIEW::pixel_t pixel
                  , std::size_t            line_width )
{
   drawRectangle( create_ipl_image( view )
                , start
                , end
                , pixel
                , static_cast<int>( line_width ));
}

/// circle

template< class PIXEL >
void drawCircle( ipl_image_wrapper& ipl_image
               , point_t            center
               , std::size_t        radius
               , PIXEL              color
               , std::size_t        line_width )
{
   cvCircle( ipl_image.get()
           , make_cvPoint( center )
           , static_cast<int>( radius )
           , make_cvScalar( color )
           , static_cast<int>( line_width ));
}

template<class VIEW>
void drawCircle( VIEW&                  view
               , point_t                center
               , std::size_t            radius
               , typename VIEW::pixel_t color
               , std::size_t            line_width )
{
   drawCircle( create_ipl_image( view )
             , center
             , static_cast<int>( radius )
             , color
             , static_cast<int>( line_width ));
}

/// line

struct eight_connected_Line{};
struct four_connected_Line{};
struct anti_aliased_Line{};

typedef boost::mpl::map< boost::mpl::pair< eight_connected_Line, boost::mpl::int_< 8    > >
                       , boost::mpl::pair< four_connected_Line , boost::mpl::int_< 4    > >
                       , boost::mpl::pair< anti_aliased_Line   , boost::mpl::int_<CV_AA > > 
                       > line_type_map;


template< class PIXEL
        , class LINE_TYPE
        >
void drawLine( ipl_image_wrapper& ipl_image
             , point_t            start
             , point_t            end
             , PIXEL              color
             , std::size_t        line_width
             , LINE_TYPE                      )
{
   cvLine( ipl_image.get()
         , make_cvPoint( start )
         , make_cvPoint( end )
         , make_cvScalar( color )
         , static_cast<int>( line_width )
         , boost::mpl::at< line_type_map
                         , LINE_TYPE
                         >::type::value   );
}

template< class VIEW >
void drawLine( VIEW&                  view
             , point_t                start
             , point_t                end
             , typename VIEW::pixel_t color
             , std::size_t            line_width )
{
   drawLine( create_ipl_image( view )
           , start
           , end
           , color
           , static_cast<int>( line_width ));
}

/// polyline

template<class PIXEL>
void drawPolyLine( ipl_image_wrapper& ipl_image
                 , const curve_vec_t& curves
                 , bool               is_closed
                 , PIXEL              color
                 , std::size_t        line_width )
{
   const std::size_t num_curves = curves.size();

   boost::scoped_array<int> num_points_per_curve( new int[num_curves] );

   std::size_t total_num_points = 0;
   for( std::size_t i = 0; i < num_curves; ++i )
   {
      num_points_per_curve[i] = static_cast<int>( curves[i].size() );
   }

   // The curve array vector will deallocate all memory by itself.
   cvpoint_array_vec_t pp( num_curves );

   CvPoint** curve_array = new CvPoint*[num_curves];

   for( std::size_t i = 0; i < num_curves; ++i )
   {
      pp[i] = make_cvPoint_array( curves[i] );

      curve_array[i] = pp[i].get();
   }
   
   cvPolyLine( ipl_image.get()
             , curve_array  // needs to be pointer to C array of CvPoints.
             , num_points_per_curve.get()// int array that contains number of points of each curve.
             , static_cast<int>( curves.size() )
             , is_closed
             , make_cvScalar( color )
             , static_cast<int>( line_width ));

}

template<class VIEW>
void drawPolyLine( VIEW&                  view
                 , const curve_vec_t&     curves
                 , bool                   is_closed
                 , typename VIEW::pixel_t color
                 , std::size_t            line_width )
{
   drawPolyLine( create_ipl_image( view )
               , curves
               , is_closed
               , color
               , line_width               );
}

template< class PIXEL >
void drawFillPoly( ipl_image_wrapper& ipl_image
                 , const curve_vec_t& curves
                 , PIXEL              color     )
{
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
   
   cvFillPoly( ipl_image.get()
             , curve_array                // needs to be pointer to C array of CvPoints.
             , num_points_per_curve.get() // int array that contains number of points of each curve.
             , curves.size()
             , make_cvScalar( color ));
}

template<class VIEW>
void drawFillPoly( VIEW&                  view
                 , const curve_vec_t&     curves
                 , typename VIEW::pixel_t color   )
{
   drawFillPoly( create_ipl_image( view )
               , curves
               , color                    );
}


} // namespace opencv
} // namespace gil
} // namespace boost


#endif // DRAWING_HPP
