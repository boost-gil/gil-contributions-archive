//#define BOOST_TEST_MODULE targa_read_test_module
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/targa_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef targa_tag tag_t;

BOOST_AUTO_TEST_SUITE( targa_test )

template< typename Image >
void write( Image&        img
          , const string& file_name
          )
{
    write_view( targa_out + file_name
              , view( img )
              , tag_t()
              );
}

BOOST_AUTO_TEST_CASE( read_header_test )
{
    {
        image_read_info< tag_t > info = read_image_info( targa_filename
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._header_size     , 18  );
        BOOST_CHECK_EQUAL( info._offset          , 18  );
        BOOST_CHECK_EQUAL( info._color_map_type  , 0   );
        BOOST_CHECK_EQUAL( info._image_type      , 10  );
        BOOST_CHECK_EQUAL( info._color_map_start , 0   );
        BOOST_CHECK_EQUAL( info._color_map_length, 0   );
        BOOST_CHECK_EQUAL( info._color_map_depth , 0   );
        BOOST_CHECK_EQUAL( info._x_origin        , 0   );
        BOOST_CHECK_EQUAL( info._y_origin        , 0   );
        BOOST_CHECK_EQUAL( info._width           , 124 );
        BOOST_CHECK_EQUAL( info._height          , 124 );
        BOOST_CHECK_EQUAL( info._bits_per_pixel  , 24  );
        BOOST_CHECK_EQUAL( info._descriptor      , 0   );
    }
}

BOOST_AUTO_TEST_CASE( read_reference_images_test )
{
    // 24BPP_compressed.tga
    {
        rgb8_image_t img;
        
        read_image( targa_in + "24BPP_compressed.tga", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 124 );
        BOOST_CHECK_EQUAL( view( img ).height(), 124 );

        write( img, "24BPP_compressed_out.tga" );
    }

    // 24BPP_uncompressed.tga
    {
        rgb8_image_t img;
        
        read_image( targa_in + "24BPP_uncompressed.tga", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 124 );
        BOOST_CHECK_EQUAL( view( img ).height(), 124 );

        write( img, "24BPP_uncompressed_out.tga" );
    }

    // 32BPP_compressed.tga
    {
        rgba8_image_t img;
        
        read_image( targa_in + "32BPP_compressed.tga", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 124 );
        BOOST_CHECK_EQUAL( view( img ).height(), 124 );

        write( img, "32BPP_compressed_out.tga" );
    }

    // 32BPP_uncompressed.tga
    {
        rgba8_image_t img;
        
        read_image( targa_in + "32BPP_uncompressed.tga", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 124 );
        BOOST_CHECK_EQUAL( view( img ).height(), 124 );

        write( img, "32BPP_uncompressed_out.tga" );
    }
}

BOOST_AUTO_TEST_CASE( partial_image_test )
{
    const std::string filename( targa_in + "24BPP_compressed.tga" );

    {
        rgb8_image_t img;
        read_image( filename
                  , img
                  , image_read_settings< targa_tag >( point_t( 0, 0 ), point_t( 50, 50 ) )
                  );


        write_view( targa_out + "targa_partial.tga"
                  , view( img )
                  , tag_t()
                  );
    }
}

BOOST_AUTO_TEST_SUITE_END()
