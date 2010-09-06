
/**************************************************
 *
 * This test file will only test the library's
 * interface. It's more of a compile time test
 * than a runtime test.
 *
 **************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <fstream>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "mandel_view.hpp"
#include "paths.hpp"
#include "subimage_test.hpp"

using namespace std;
using namespace boost;
using namespace gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_AUTO_TEST_CASE( read_image_info_test )
{
    {
        image_read_info< tag_t > info = read_image_info( tiff_filename
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200u );
        BOOST_CHECK_EQUAL( info._height, 133u );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );
        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200u );
        BOOST_CHECK_EQUAL( info._height, 133u );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        image_read_info< tag_t > info = read_image_info( file
                                                       , tag_t() );
        
        BOOST_CHECK_EQUAL( info._width , 200u );
        BOOST_CHECK_EQUAL( info._height, 133u );
    }
}

BOOST_AUTO_TEST_CASE( read_image_test )
{
    {
        rgb8_image_t img;
        read_image( tiff_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {

        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( tiff_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img;
        read_and_convert_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }
}

BOOST_AUTO_TEST_CASE( read_view_test )
{
    {
        rgb8_image_t img( 200, 133 );
        read_view( tiff_filename, view( img ), tag_t() );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img( 200, 133 );
        read_view( in, view( img ), tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img( 200, 133 );
        read_view( file, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_view_test )
{
    {
        rgb8_image_t img( 200, 133 );
        read_and_convert_view( tiff_filename, view( img ), tag_t() );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img( 200, 133 );
        read_and_convert_view( in, view( img ), tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200u );
        BOOST_CHECK_EQUAL( img.height(), 133u );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img( 200, 133 );
        read_and_convert_view( file, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    rgb8_image_t img( 320, 240 );

    {
        string filename( tiff_out + "write_test_string.tif" );

        write_view( filename
                  , create_mandel_view( 320, 240
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        string filename( tiff_out + "write_test_ofstream.tif" );
        ofstream out( filename.c_str(), ios_base::binary );

        write_view( out
                  , create_mandel_view( 320, 240
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        string filename( tiff_out + "write_test_tiff.tif" );
        TIFF* file = TIFFOpen( filename.c_str(), "w" );
        
        write_view( file
                  , create_mandel_view( 320, 240
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , tag_t()
                  );
    }

    {
        string filename( tiff_out + "write_test_info.tif" );

        image_write_info< tiff_tag > info;
        write_view( filename
                  , create_mandel_view( 320, 240
                                      , rgb8_pixel_t( 0,   0, 255 )
                                      , rgb8_pixel_t( 0, 255,   0 )
                                      )
                  , info
                  );
    }
}

BOOST_AUTO_TEST_CASE( stream_test )
{
    // 1. Read an image.
    ifstream in( tiff_filename.c_str(), ios::binary );

    rgb8_image_t img;
    read_image( in, img, tag_t() );

    // 2. Write image to in-memory buffer.
    stringstream out_buffer( ios_base::in | ios_base::out | ios_base::binary );
    write_view( out_buffer, view( img ), tag_t() );

    // 3. Copy in-memory buffer to another.
    stringstream in_buffer( ios_base::in | ios_base::out | ios_base::binary );
    in_buffer << out_buffer.rdbuf();

    // 4. Read in-memory buffer to gil image
    rgb8_image_t dst;
    read_image( in_buffer, dst, tag_t() );

    // 5. Write out image.
    string filename( tiff_out + "stream_test.tif" );
    ofstream out( filename.c_str(), ios_base::binary );
    write_view( out, view( dst ), tag_t() );
}

BOOST_AUTO_TEST_CASE( stream_test_2 )
{
    filebuf in_buf;
    if( !in_buf.open( tiff_filename.c_str(), ios::in | ios::binary ) )
    {
        BOOST_CHECK( false );
    }

    istream in( &in_buf );

    rgb8_image_t img;
    read_image( in, img, tag_t() );
}

BOOST_AUTO_TEST_CASE( subimage_test )
{
    run_subimage_test< rgb8_image_t, tag_t >( tiff_filename
                                            , point_t(  0,  0 )
                                            , point_t( 50, 50 )
                                            );

    run_subimage_test< rgb8_image_t, tag_t >( tiff_filename
                                            , point_t(  50,  50 )
                                            , point_t(  50,  50 )
                                            );
}

BOOST_AUTO_TEST_CASE( subimage_test_2 )
{
    typedef bit_aligned_image1_type< 3, gray_layout_t >::type image_t;
    image_t img1, img2, img3;

    point_t top_left( 10, 10 );
    point_t dim( 32, 32 );

    image_read_settings< tag_t > settings( top_left, dim );

    read_image( tiff_in_GM + "tiger-minisblack-strip-03.tif"
              , img1
              , settings
              );

    read_image( tiff_in_GM + "tiger-minisblack-tile-03.tif"
              , img2
              , settings
              );

    read_image( tiff_in_GM + "tiger-minisblack-strip-03.tif"
              , img3
              , tag_t()
              );
    
    BOOST_CHECK( equal_pixels( const_view( img1 ), const_view( img2 )));
    BOOST_CHECK( equal_pixels( const_view( img1 ), subimage_view( view( img3 ), top_left, dim )));

}

BOOST_AUTO_TEST_CASE( dynamic_image_test )
{
    typedef mpl::vector< gray8_image_t
                       , gray16_image_t
                       , rgb8_image_t
                       , gil::detail::gray1_image_t
                       > my_img_types;


    any_image< my_img_types > runtime_image;

    read_image( tiff_filename.c_str()
              , runtime_image
              , tag_t()
              );

    write_view( tiff_out + "dynamic_image_test.tif"
              , view( runtime_image )
              , tag_t()
              );
}

BOOST_AUTO_TEST_SUITE_END()
