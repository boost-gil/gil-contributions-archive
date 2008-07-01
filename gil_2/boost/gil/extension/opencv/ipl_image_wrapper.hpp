#ifndef IPL_IMAGE_WRAPPER_HPP
#define IPL_IMAGE_WRAPPER_HPP

#include <boost\gil\gil_all.hpp>

#include "utilities.hpp"

namespace boost { namespace gil { namespace opencv {

template < typename Channel > struct ipl_channel_type : boost::mpl::false_ {};
template<> struct ipl_channel_type< bits8 >   : boost::mpl::int_< IPL_DEPTH_8U  > {};
template<> struct ipl_channel_type< bits16 >  : boost::mpl::int_< IPL_DEPTH_16U > {};
template<> struct ipl_channel_type< bits32f > : boost::mpl::int_< IPL_DEPTH_32F > {};
template<> struct ipl_channel_type< double >  : boost::mpl::int_< IPL_DEPTH_64F > {};
template<> struct ipl_channel_type< bits8s >  : boost::mpl::int_< IPL_DEPTH_8S  > {};
template<> struct ipl_channel_type< bits16s > : boost::mpl::int_< IPL_DEPTH_16S > {};
template<> struct ipl_channel_type< bits32s > : boost::mpl::int_< IPL_DEPTH_32S > {};

class ipl_image_wrapper
{
public:
    ipl_image_wrapper( IplImage* img ) : _img( img ) {}

    ~ipl_image_wrapper()
    {
/*
        if( _img )
        {
            cvReleaseData( &_img );
            cvReleaseImageHeader( &_img );
        }
*/
    }

    IplImage* get() { return _img; }

    const IplImage* get() const { return _img; }
   
private:

   IplImage* _img;
};

template< typename View >
inline
ipl_image_wrapper create_ipl_image( View view )
{
    typedef typename channel_type< View >::type channel_t;

    IplImage* img;

    if(( img = cvCreateImageHeader( make_cvSize( view.dimensions() )
                                  , ipl_channel_type<channel_t>::type::value
                                  , num_channels<View>::value
                                  )) == NULL )
    {
        throw std::runtime_error( "Cannot create IPL image." );
    }

    cvSetData( img
             , &view.begin()[0]
             , num_channels<View>::value * view.width() * sizeof( channel_t ) );

    return ipl_image_wrapper( img );
}

} // namespace opencv
} // namespace gil
} // namespace boost

#endif // IPL_IMAGE_WRAPPER_HPP
