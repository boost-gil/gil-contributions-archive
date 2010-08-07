#include "stdafx.h"

#include <boost\test\unit_test.hpp>

#include <geometry\geometry.hpp>


#include <boost\gil\extension\io_new\png_all.hpp>

using namespace boost;
using namespace gil;
//using namespace boost::gil::opencv;

template< typename Geometry, typename View >
struct draw {};

template< typename View, typename Point >
struct draw< geometry::box< Point >, View > { static void d() {} };



BOOST_AUTO_TEST_CASE( test_geometry )
{
	typedef geometry::point_xy<double> P;

	geometry::box< P > b( P( 10, 10 )
	                    , P( 35, 70 )
	                    );

    rgb8_image_t img;
    int i;
    //draw( i, view( img ) );

    draw< geometry::box< P >, double >::d();
    


    //write_view( "..\\out\\geometry.png", view( dst ), png_tag() );
}
