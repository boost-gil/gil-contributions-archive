#ifndef IPL_IMAGE_WRAPPER_HPP
#define IPL_IMAGE_WRAPPER_HPP

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

#include "utilities.hpp"

namespace boost { namespace gil {
namespace opencv {

// taken from Hirotaka's stllcv code
typedef boost::mpl::map<
	 boost::mpl::pair<gil::bits8	,  boost::mpl::int_<IPL_DEPTH_8U> >
,	 boost::mpl::pair<gil::bits16	,  boost::mpl::int_<IPL_DEPTH_16U> >
,	 boost::mpl::pair<gil::bits32f	,  boost::mpl::int_<IPL_DEPTH_32F> >
,	 boost::mpl::pair<double      ,  boost::mpl::int_<IPL_DEPTH_64F> >
,	 boost::mpl::pair<gil::bits8s	,  boost::mpl::int_<IPL_DEPTH_8S> >
,	 boost::mpl::pair<gil::bits16s	,  boost::mpl::int_<IPL_DEPTH_16S> >
,	 boost::mpl::pair<gil::bits32s	,  boost::mpl::int_<IPL_DEPTH_32S> >
    > ipl_depth_map_from_channel_t_map;

struct undefined {};

// I'm using a tyedef instead of a static constant so we can have
// type safety and generate an exception when we need to.
template < typename Depth > struct ipl_depth_type { typedef undefined type; };
struct ipl_depth_type< gil::bits8 >   { typedef boost::mpl::int_<IPL_DEPTH_8U>  type; };
struct ipl_depth_type< gil::bits16 >  { typedef boost::mpl::int_<IPL_DEPTH_16U> type; };
struct ipl_depth_type< gil::bits32f > { typedef boost::mpl::int_<IPL_DEPTH_32F> type; };
struct ipl_depth_type< double >       { typedef boost::mpl::int_<IPL_DEPTH_64F> type; };
struct ipl_depth_type< gil::bits8s >  { typedef boost::mpl::int_<IPL_DEPTH_8S>  type; };
struct ipl_depth_type< gil::bits16s > { typedef boost::mpl::int_<IPL_DEPTH_16S> type; };
struct ipl_depth_type< gil::bits32s > { typedef boost::mpl::int_<IPL_DEPTH_32S> type; };


typedef boost::mpl::map<
	 boost::mpl::pair<gil::gray_t	,  boost::mpl::int_<1> >
,	 boost::mpl::pair<gil::rgb_t	,  boost::mpl::int_<3> >
,	 boost::mpl::pair<gil::rgba_t	,  boost::mpl::int_<4> >
    > ipl_nchannels_from_gil_color_space_map;

class ipl_image_wrapper
{
public:
   ipl_image_wrapper( IplImage* img ) : _img( img ) {}

   ~ipl_image_wrapper() { /* if( _img ) cvReleaseImage( &_img ); */ }

   IplImage* get() { return _img; }

   const IplImage* get() const { return _img; }
   
private:

   IplImage* _img;
};

template<class VIEW>
inline
ipl_image_wrapper create_ipl_image( VIEW view )
{
	typedef typename channel_type<VIEW>::type channel_t;
   typedef typename color_space_type<VIEW>::type color_space_t;

   IplImage* img;

   img = cvCreateImageHeader( make_cvSize( view.dimensions() )
		                      , ipl_depth_type<channel_t>::type::value
                            , boost::mpl::at< ipl_nchannels_from_gil_color_space_map
                                            , color_space_t>::type::value             );

   if( !img )
   {
      throw std::runtime_error( "Cannot create IPL image." );
   }

   cvSetData( img
            , &view.begin()[0]
            , num_channels<VIEW>::value * view.width() * sizeof( channel_t ) );

   return ipl_image_wrapper( img );
}


} // namespace opencv
} // namespace gil
} // namespace boost

#endif