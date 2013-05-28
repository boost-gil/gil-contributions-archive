#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>


namespace boost { namespace gil {

template< typename Iterator > // Iterator will be planar_pixel_iterator
struct memunit_2_step_fn
{
    typedef std::ptrdiff_t luma_difference_type;   // component_1
    typedef std::ptrdiff_t chroma_difference_type; // component_2

    typedef std::pair< luma_difference_type
                     , chroma_difference_type
                     > difference_type;

    // Constructor
    memunit_2_step_fn( luma_difference_type   step_1
                     , chroma_difference_type step_2
                     )
    : _step( step_1, step_2 )
    {}
    
private:
    GIL_CLASS_REQUIRE(Iterator, boost::gil, MemoryBasedIteratorConcept)

    difference_type _step;
};

/////////////////////////////
//  MemoryBasedIteratorConcept
/////////////////////////////

// memunit_step
template < typename ChannelPtr
         , typename ColorSpace
         >
inline
typename memunit_2_step_fn< planar_pixel_iterator< ChannelPtr, ColorSpace > >::difference_type
memunit_step( const planar_pixel_iterator< ChannelPtr, ColorSpace>&, int )
{
    typedef typename memunit_2_step_fn::difference_type return_t;

    return return_t( sizeof( typename std::iterator_traits< ChannelPtr >::value_type )
                   , sizeof( typename std::iterator_traits< ChannelPtr >::value_type )
                   );
}

// memunit_distance
template < typename ChannelPtr
         , typename ColorSpace
         >
inline
typename memunit_2_step_fn< planar_pixel_iterator< ChannelPtr, ColorSpace > >::difference_type
memunit_distance( const planar_pixel_iterator< ChannelPtr, ColorSpace >& p1
                , const planar_pixel_iterator< ChannelPtr, ColorSpace >& p2
                )
{
    assert(  memunit_distance( gil::at_c< 1 >( p1 ),gil::at_c< 1 >( p2 ))
          == memunit_distance( gil::at_c< 2 >( p1 ),gil::at_c< 2 >( p2 ))
          );

    typedef planar_pixel_iterator< ChannelPtr, ColorSpace > pixel_t;
    BOOST_STATIC_ASSERT(( gil::num_channels< pixel_t >::value == 3 ));

    typedef typename memunit_2_step_fn::difference_type return_t;

    return return_t( memunit_distance( gil::at_c< 0 >( p1 ),gil::at_c< 0 >( p2 ))
                   , memunit_distance( gil::at_c< 1 >( p1 ),gil::at_c< 1 >( p2 ))
                   );
}

// memunit_advance
template < typename ChannelPtr
         , typename ColorSpace
         >
inline
void
memunit_advance( planar_pixel_iterator< ChannelPtr, ColorSpace >& p
               , typename memunit_2_step_fn< planar_pixel_iterator< ChannelPtr, ColorSpace > >::difference_type      diff
               )
{
    typedef planar_pixel_iterator< ChannelPtr, ColorSpace > pixel_t;
    BOOST_STATIC_ASSERT(( gil::num_channels< pixel_t >::value == 3 ));

    memunit_advance( gil::at_c< 0 >( p ), diff.first  );
    memunit_advance( gil::at_c< 1 >( p ), diff.second );
    memunit_advance( gil::at_c< 2 >( p ), diff.second );
}

// memunit_advanced
template < typename ChannelPtr
         , typename ColorSpace
         >
inline
planar_pixel_iterator< ChannelPtr, ColorSpace >
memunit_advanced( const planar_pixel_iterator< ChannelPtr, ColorSpace >& p
                , const typename memunit_2_step_fn< planar_pixel_iterator< ChannelPtr, ColorSpace > >::difference_type&     diff
                )
{
    typedef planar_pixel_iterator< ChannelPtr, ColorSpace > iterator_t;
    BOOST_STATIC_ASSERT(( gil::num_channels< iterator_t >::value == 3 ));

    pixel_t ret = p;
    memunit_advance(ret, diff);

    return ret;
}

// memunit_advanced_ref
template < typename ChannelPtr
         , typename ColorSpace
         >
inline
planar_pixel_reference< typename std::iterator_traits< ChannelPtr >::reference
                      , ColorSpace
                      >
memunit_advanced_ref( const planar_pixel_iterator<ChannelPtr,ColorSpace>& ptr
                    , const typename memunit_2_step_fn< planar_pixel_iterator< ChannelPtr, ColorSpace > >::difference_type& diff
                    )
{
    typedef planar_pixel_iterator< ChannelPtr, ColorSpace > iterator_t;
    typedef planar_pixel_reference< typename std::iterator_traits< ChannelPtr >::reference
                                 , ColorSpace
                                 > reference_t;

    BOOST_STATIC_ASSERT(( gil::num_channels< iterator_t >::value == 3 ));

    iterator_t it( ptr );
    memunit_advance( it, diff );

    return reference_t( gil::at_c< 0 >( it )
                      , gil::at_c< 1 >( it )
                      , gil::at_c< 2 >( it )
                      );
}

////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup ImageModel PixelBasedModel
/// \brief container interface over image view. Models ImageConcept, PixelBasedConcept
///
/// 
///
////////////////////////////////////////////////////////////////////////////////////////
//template< Allocator = std::allocator< unsigned char > >
//class subchroma_image
//{
//public:
//
//    typedef unsigned char channel_t;
//    typedef ycbcr_601_8_planar_pixel_t pixel_t;
//    
//    typedef planar_pixel_iterator< channel_t*, pixel_t::layout_t  > x_iterator_t;
//    typedef memunit_2_step_fn< x_iterator_t > step_iterator_fn_t;
//
//    typedef memory_based_step_iterator< x_iterator_t, step_iterator_fn_t > step_iterator_t;
//    typedef memory_based_2d_locator< step_iterator_t > locator_t;
//    typedef image_view< locator_t > view_t;
//};

} // gil
} // boost

using namespace std;
using namespace boost;
using namespace gil;

/*
template < typename ChannelReference // e.g. unsigned char&
         , typename ColorSpace       // 
         >        
struct subchroma_pixel_reference 
   : public detail::homogeneous_color_base<ChannelReference,layout<ColorSpace>,mpl::size<ColorSpace>::value>
*/




BOOST_AUTO_TEST_SUITE( gil_test )

BOOST_AUTO_TEST_CASE( small_test )
{
    // IMC4
    // Y - NxN
    // U,V - ( N / 2 ) x ( N / 2 )
    // YYYYVUVU

    // 10 x 10 x 1 byte = 100 bytes
    // 10 x 5  x 1 byte =  50 bytes

    // planar image with two planes

    rgb8_image_t n( 640, 480, rgb8_pixel_t( 255, 128, 20 ));
    auto n_it = view(n).at( 0, 0 );
    auto  n_p = *n_it;


    rgb8_planar_image_t o( 640, 480, rgb8_pixel_t( 255, 128, 20 ));
    auto o_it = view(o).at( 0, 0 );
    auto  o_p = *o_it;


    auto n_loc = view(n).xy_at( 0, 0 );
    auto n_x_it = n_loc.x();
    auto n_y_it = n_loc.y();
    
    auto o_loc = view(o).xy_at( 0, 0 );
    auto o_x_it = o_loc.x();
    auto o_y_it = o_loc.y();

    //subchroma_image img;

}

BOOST_AUTO_TEST_SUITE_END()
