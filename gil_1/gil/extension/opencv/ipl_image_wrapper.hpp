#ifndef IPL_IMAGE_WRAPPER_HPP
#define IPL_IMAGE_WRAPPER_HPP

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

#include "utilities.hpp"

namespace opencv {

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

class ipl_image_wrapper
{
public:
   ipl_image_wrapper( IplImage* img ) : _img( img ) {}

   ~ipl_image_wrapper() { if( _img ) cvReleaseImage( &_img); }

   IplImage* get() { return _img; }

   const IplImage* get() const { return _img; }
   
private:

   IplImage* _img;
};

template<class VIEW>
inline
ipl_image_wrapper create_ipl_image( VIEW view )
{
   typedef typename VIEW::channel_t channel_t;
   typedef typename VIEW::color_space_t color_space_t;

   IplImage* img;

   img = cvCreateImage( make_cvSize( view.dimensions() )
		               , boost::mpl::at< ipl_depth_map_from_channel_t_map
                                       , channel_t>::type::value
		               , boost::mpl::at< ipl_nchannels_from_gil_color_space_map
                                     , color_space_t>::type::value             );

   if( !img )
   {
      throw std::runtime_error( "Cannot create IPL image." );
   }

   img->imageData = (char*) ( &view.begin()[0] );

   return ipl_image_wrapper( img );
}


} // namespace opencv

#endif