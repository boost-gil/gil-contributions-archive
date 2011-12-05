//#define BOOST_TEST_MODULE pnm_read_test_module
#include <boost/test/unit_test.hpp>

#include <boost/gil/extension/io_new/pnm_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef pnm_tag tag_t;

BOOST_AUTO_TEST_SUITE( pnm_test )

template< typename Image >
void write( Image&        img
          , const string& file_name
          )
{
    write_view( pnm_out + file_name
              , view( img )
              , tag_t()
              );
}

BOOST_AUTO_TEST_CASE( read_header_test )
{
    {
        image_read_info< tag_t > info = read_image_info( pnm_filename
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._type     , pnm_image_type::color_asc_t::value );
        BOOST_CHECK_EQUAL( info._width    , 256u                        );
        BOOST_CHECK_EQUAL( info._height   , 256u                        );
        BOOST_CHECK_EQUAL( info._max_value, 255u                        );
    }
}

BOOST_AUTO_TEST_CASE( read_reference_images_test )
{
    // p1.pnm
    {
        gray8_image_t img;
        
        read_image( pnm_in + "p1.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 200u );
        BOOST_CHECK_EQUAL( view( img ).height(), 200u );

        write( img, "p1.pnm" );
    }

    // p2.pnm
    {
        gray8_image_t img;
        
        read_image( pnm_in + "p2.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 200u );
        BOOST_CHECK_EQUAL( view( img ).height(), 200u );

        write( img, "p2.pnm" );
    }

    // p3.pnm
    {
        rgb8_image_t img;
        
        read_image( pnm_in + "p3.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 256u );
        BOOST_CHECK_EQUAL( view( img ).height(), 256u );

        write( img, "p3.pnm" );
    }

    // p4.pnm
    {
        gray1_image_t img;
        
        read_image( pnm_in + "p4.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 200u );
        BOOST_CHECK_EQUAL( view( img ).height(), 200u );

        write( img, "p4.pnm" );
    }

    // p5.pnm
    {
        gray8_image_t img;
        
        read_image( pnm_in + "p5.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 200u );
        BOOST_CHECK_EQUAL( view( img ).height(), 200u );

        write( img, "p5.pnm" );
    }

    // p6.pnm
    {
        rgb8_image_t img;
        
        read_image( pnm_in + "p6.pnm", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 256u );
        BOOST_CHECK_EQUAL( view( img ).height(), 256u );

        write( img, "p6.pnm" );
    }
}

BOOST_AUTO_TEST_SUITE_END()
