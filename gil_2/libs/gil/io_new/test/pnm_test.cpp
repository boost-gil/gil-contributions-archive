#include <fstream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/type_traits/is_same.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef pnm_tag tag_t;

BOOST_AUTO_TEST_CASE( pnm_read_image_info_using_string )
{
    {
        image_read_info< pnm_tag > info = read_image_info( pnm_filename
                                                         , tag_t() );
        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 200 );
    }

    {
        ifstream in( pnm_filename.c_str(), ios::binary );

        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 200 );
    }

    {
        FILE* file = fopen( pnm_filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = read_image_info( file
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_read_image_test )
{
    {
        rgb8_image_t img;
        read_image( pnm_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }

    {
        ifstream in( pnm_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }

    {
        FILE* file = fopen( pnm_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( pnm_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }

    {
        ifstream in( pnm_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }

    {
        FILE* file = fopen( pnm_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_and_convert_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 200 );
    }
}

BOOST_AUTO_TEST_CASE( pnm_read_view_test )
{
    {
        rgb8_image_t img( 200, 200 );
        read_view( pnm_filename, view( img ), tag_t() );
    }

    {
        ifstream in( pnm_filename.c_str(), ios::binary );

        rgb8_image_t img( 200, 200 );
        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( pnm_filename.c_str(), "rb" );
        
        rgb8_image_t img( 200, 200 );
        read_view( file, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( pnm_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 200, 200 );
        read_and_convert_view( pnm_filename, view( img ), tag_t() );
    }

    {
        ifstream in( pnm_filename.c_str(), ios::binary );

        rgb8_image_t img( 200, 200 );
        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( pnm_filename.c_str(), "rb" );
        
        rgb8_image_t img( 200, 200 );

        read_and_convert_view( file
                             , view( img )
                             , tag_t()
                             );
    }
}

BOOST_AUTO_TEST_CASE( pnm_write_view_test )
{
    {
        string filename( pnm_out + "write_test_string.pnm" );

        gray8_image_t img( 200, 200 );
        write_view( filename, view( img ), tag_t() );
    }

    {
        string filename( pnm_out + "write_test_ofstream.pnm" );

        ofstream out( filename.c_str(), ios::out | ios::binary );

        rgb8_image_t img( 200, 200 );
        write_view( out, view( img ), tag_t() );
    }

    {
        string filename( pnm_out + "write_test_file.pnm" );

        FILE* file = fopen( filename.c_str(), "wb" );
        
        rgb8_image_t img( 200, 200 );
        write_view( file, view( img ), tag_t() );
    }

    {
        string filename( pnm_out + "write_test_info.pnm" );
        FILE* file = fopen( filename.c_str(), "wb" );

        image_write_info< pnm_tag > info;
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), info );
    }
}

BOOST_AUTO_TEST_CASE( pnm_stream_test )
{
    // 1. Read an image.
    ifstream in( pnm_filename.c_str(), ios::binary );

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
    string filename( pnm_out + "stream_test.pnm" );
    ofstream out( filename.c_str(), ios_base::binary );
    write_view( out, view( dst ), tag_t() );
}
