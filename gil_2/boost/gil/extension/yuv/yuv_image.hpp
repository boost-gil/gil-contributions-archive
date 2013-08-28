#ifndef GIL_YUV_IMAGE_HPP
#define GIL_YUV_IMAGE_HPP

#include <boost/gil/image.hpp>
#include <boost/gil/extension/toolbox/color_spaces/ycbcr.hpp>

#include <boost/gil/extension/yuv/yuv_planar_pixel_iterator.hpp>


namespace boost {
namespace gil {

template< int J
        , int a
        , int b
        >
struct Scaling_Factors
{
    BOOST_STATIC_ASSERT(( mpl::equal_to< mpl::int_< J >, mpl::int_< 4 > >::type::value ));

    BOOST_STATIC_ASSERT(( mpl::or_< mpl::equal_to< mpl::int_< a >, mpl::int_< 4 > >
                                  , mpl::or_< mpl::equal_to< mpl::int_< a >, mpl::int_< 2 > >
                                            , mpl::equal_to< mpl::int_< a >, mpl::int_< 1 > >
                                            >
                                  >::type::value
                       ));

    BOOST_STATIC_ASSERT(( mpl::or_< mpl::equal_to< mpl::int_< b >, mpl::int_< 4 > >
                                  , mpl::or_< mpl::equal_to< mpl::int_< b >, mpl::int_< 2 > >
                                            , mpl::or_< mpl::equal_to< mpl::int_< b >, mpl::int_< 1 > >
                                                      , mpl::equal_to< mpl::int_< b >, mpl::int_< 0 > >
                                                      >
                                            >
                                  >::type::value
                       ));

    BOOST_STATIC_CONSTANT( int, ss_X = ( mpl::divides< mpl::int_< J >
                                                     , mpl::int_< a >
                                                     >::type::value )
                         );

    BOOST_STATIC_CONSTANT( int, ss_Y = ( mpl::if_< mpl::equal_to< mpl::int_< b >, mpl::int_< 0 > >
                                                 , mpl::int_< 2 >
                                                 , typename mpl::if_< mpl::equal_to< mpl::int_< a >, mpl::int_< b > >
                                                                    , mpl::int_< 1 >
                                                                    , mpl::int_< 4 >
                                                                    >::type
                                                 >::type::value )
                         );
};

///////////////////////////
// get_channel_ptr
///////////////////////////
template< typename Pixel >
struct get_channel_ptr
{
    typedef typename channel_traits< typename channel_type< Pixel >::type >::pointer type;
};


///////////////////////////
// is_supported_yuv_type
///////////////////////////
template< typename Factors > struct is_supported_yuv_type : mpl::false_ {};
template<> struct is_supported_yuv_type< mpl::vector_c< int, 4, 4, 4 > > : mpl::true_ {};
template<> struct is_supported_yuv_type< mpl::vector_c< int, 4, 2, 2 > > : mpl::true_ {};
template<> struct is_supported_yuv_type< mpl::vector_c< int, 4, 2, 0 > > : mpl::true_ {};

///////////////////////////
// get_yuv_x_iterator
///////////////////////////
template< typename ChannelPtr
        , typename ColorSpace
        , typename Factors
        >
struct get_yuv_x_iterator {};


template< typename ChannelPtr
        , typename ColorSpace
        >
struct get_yuv_x_iterator< ChannelPtr
                         , ColorSpace
                         , mpl::vector_c< int, 4, 4, 4 >
                         >
{
    typedef planar_pixel_iterator< ChannelPtr
                                 , ColorSpace
                                 > type;
};

template< typename ChannelPtr
        , typename ColorSpace
        >
struct get_yuv_x_iterator< ChannelPtr
                         , ColorSpace
                         , mpl::vector_c< int, 4, 2, 0 >
                         >
{
    typedef yuv_planar_pixel_iterator< ChannelPtr
                                     , ColorSpace
                                     > type;
};

///////////////////////////
// get_yuv_view
///////////////////////////
template< typename Pixel
        , typename Factors
        , class Enable = void
        >
struct get_yuv_view {};

template< typename Pixel
        , typename Factors
        >
struct get_yuv_view< Pixel
                   , Factors
                   , typename enable_if< is_supported_yuv_type< Factors > >::type
                   >
{
    typedef typename get_channel_ptr< Pixel >::type channel_ptr_t;
    typedef typename color_space_type< Pixel >::type color_space_t;

    typedef typename get_yuv_x_iterator< channel_ptr_t
                                       , color_space_t
                                       , Factors
                                       >::type x_iterator_t;

    typedef typename type_from_x_iterator< x_iterator_t >::view_t type;
};

///////////////////////////
// is_supported_yuv_type
///////////////////////////
struct IMC1
{
    template< typename ChannelPtr
            , int SS_X
            , int SS_Y
            >
    static
    void set_planes( std::size_t width
                   , std::size_t height
                   , std::size_t channel_size
                   , ChannelPtr data
                   , ChannelPtr& y_plane
                   , ChannelPtr& v_plane
                   , ChannelPtr& u_plane
                   )
    {
        y_plane = data;
        v_plane = y_plane + width * height * channel_size;
        u_plane = v_plane + ( width  / SS_X )
                          * ( height / SS_Y )
                          * channel_size;
    }
};

struct IMC2
{
    template< typename ChannelPtr
            , int SS_X
            , int SS_Y
            >
    static
    void set_planes( std::size_t width
                   , std::size_t height
                   , std::size_t channel_size
                   , ChannelPtr data
                   , ChannelPtr& y_plane
                   , ChannelPtr& v_plane
                   , ChannelPtr& u_plane
                   )
    {
        y_plane = data;
        v_plane = y_plane + width * height * channel_size;
        u_plane = v_plane + ( width / SS_X )
                          * channel_size;
    }
};


//////////////////////////////

template< typename Pixel
        , typename Factors
        , typename Format
        >
inline
typename get_yuv_view< Pixel
                     , Factors
                     >::type
yuv_view( std::size_t width
        , std::size_t height
        , typename get_channel_ptr< Pixel >::type data
        , std::ptrdiff_t rowsize_in_bytes
        )
{
    typedef Scaling_Factors< mpl::at_c< Factors, 0 >::type::value
                           , mpl::at_c< Factors, 1 >::type::value
                           , mpl::at_c< Factors, 2 >::type::value
                           > scaling_factors_t;

    typedef typename get_yuv_view< Pixel, Factors >::type view_t;

    typedef typename get_channel_ptr< Pixel >::type channel_ptr_t;

    std::size_t channel_size = 1;

    channel_ptr_t y_base, v_base, u_base;

    Format::template set_planes< channel_ptr_t
                      , scaling_factors_t::ss_X
                      , scaling_factors_t::ss_Y
                      >( width
                       , height
                       , channel_size
                       , data
                       , y_base
                       , v_base
                       , u_base
                       );


    return view_t( width
                 , height
                 , typename view_t::locator( typename view_t::x_iterator( y_base
                                                                        , v_base
                                                                        , u_base
                                                                        )
                                           , rowsize_in_bytes
                                           )
                 );
}

//
//
//
///// \ingroup ImageViewConstructors
///// \brief from raw YUV planar data
//template < typename ChannelPtr
//         , typename ColorSpace
//         >
//inline
//typename type_from_x_iterator< yuv_planar_pixel_iterator< ChannelPtr
//                                                        , ColorSpace
//                                                        >
//                             >::view_t
//planar_yuv_view( std::size_t width
//               , std::size_t height
//               , ChannelPtr y
//               , ChannelPtr v
//               , ChannelPtr u
//               , std::ptrdiff_t rowsize_in_bytes
//               )
//{
//    typedef typename type_from_x_iterator< yuv_planar_pixel_iterator< ChannelPtr
//                                                                    , ColorSpace
//                                                                    >
//                                         >::view_t RView;
//
//    return RView( width
//                , height
//                , typename RView::locator( yuv_planar_pixel_iterator< ChannelPtr
//                                                                    , ColorSpace
//                                                                    >( y, v, u )
//                                         , rowsize_in_bytes
//                                         )
//                );
//}
//
//
//template < typename ChannelPtr
//         , typename ColorSpace
//         >
//inline
//typename type_from_x_iterator< planar_pixel_iterator< ChannelPtr
//                                                    , ColorSpace
//                                                    >
//                             >::view_t
//planar_view( std::size_t width
//           , std::size_t height
//           , ChannelPtr y
//           , ChannelPtr v
//           , ChannelPtr u
//           , std::ptrdiff_t rowsize_in_bytes
//           )
//{
//    typedef typename type_from_x_iterator< planar_pixel_iterator< ChannelPtr
//                                                                , ColorSpace
//                                                                >
//                                         >::view_t RView;
//
//    return RView( width
//                , height
//                , typename RView::locator( planar_pixel_iterator< ChannelPtr
//                                                                , ColorSpace
//                                                                >( y, v, u )
//                                         , rowsize_in_bytes
//                                         )
//                );
//}


} // namespace gil
} // namespace boost


// taken from http://fourcc.org/yuv.php#IMC1

///
/// Y - y plane
/// V - v plane
/// U - u plane
/// P - Padding

///
/// YYYY
/// YYYY
/// YYYY
/// YYYY
/// VVPP
/// VVPP
/// UUPP
/// UUPP
///
///
struct IMC1 {};

///
/// YYYY
/// YYYY
/// YYYY
/// YYYY
/// VVUU
/// VVUU
///
struct IMC2 {};

/// - same as IMC1 but V and U planes are swapped
/// YYYY
/// YYYY
/// YYYY
/// YYYY
/// UUPP
/// UUPP
/// VVPP
/// VVPP
///
///
struct IMC3 {};

/// - same as IMC2 but V and U planes are swapped
/// YYYY
/// YYYY
/// YYYY
/// YYYY
/// UUVV
/// UUVV
///
struct IMC4 {};

#endif // GIL_YUV_IMAGE_HPP
