#ifndef IPL_IMAGE_WRAPPER_HPP
#define IPL_IMAGE_WRAPPER_HPP

#include "utilities.hpp"

namespace boost { namespace gil { namespace opencv {

struct undefined {};

template < typename Channel > struct ipl_channel_type { typedef undefined type; };
template<> struct ipl_channel_type< bits8 >   : public boost::mpl::int_< IPL_DEPTH_8U  > {};
template<> struct ipl_channel_type< bits16 >  : public boost::mpl::int_< IPL_DEPTH_16U >  {};
template<> struct ipl_channel_type< bits32f > : public boost::mpl::int_< IPL_DEPTH_32F > {};
template<> struct ipl_channel_type< double >  : public boost::mpl::int_< IPL_DEPTH_64F > {};
template<> struct ipl_channel_type< bits8s >  : public boost::mpl::int_< IPL_DEPTH_8S  > {};
template<> struct ipl_channel_type< bits16s > : public boost::mpl::int_< IPL_DEPTH_16S > {};
template<> struct ipl_channel_type< bits32s > : public boost::mpl::int_< IPL_DEPTH_32S > {};

template < typename Depth > struct ipl_depth_type { typedef undefined type; };
template<> struct ipl_depth_type< gil::bits8 >   : public boost::mpl::int_< IPL_DEPTH_8U  > {};
template<> struct ipl_depth_type< gil::bits16 >  : public boost::mpl::int_< IPL_DEPTH_16U > {};
template<> struct ipl_depth_type< gil::bits32f > : public boost::mpl::int_< IPL_DEPTH_32F > {};
template<> struct ipl_depth_type< double >       : public boost::mpl::int_< IPL_DEPTH_64F > {};
template<> struct ipl_depth_type< gil::bits8s >  : public boost::mpl::int_< IPL_DEPTH_8S  > {};
template<> struct ipl_depth_type< gil::bits16s > : public boost::mpl::int_< IPL_DEPTH_16S > {};
template<> struct ipl_depth_type< gil::bits32s > : public boost::mpl::int_< IPL_DEPTH_32S > {};

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

template<class VIEW>
inline
ipl_image_wrapper create_ipl_image( VIEW view )
{
    typedef typename channel_type<VIEW>::type channel_t;
    typedef typename color_space_type<VIEW>::type color_space_t;

    IplImage* img;

    if(( img = cvCreateImageHeader( make_cvSize( view.dimensions() )
                                  , ipl_depth_type<channel_t>::type::value
                                  , num_channels<VIEW>::value
                                  )) == NULL )
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

#endif // IPL_IMAGE_WRAPPER_HPP