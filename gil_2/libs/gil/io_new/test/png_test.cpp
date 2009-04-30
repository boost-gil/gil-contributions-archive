#include <fstream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/type_traits/is_same.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef png_tag tag_t;

BOOST_AUTO_TEST_CASE( png_read_image_info_using_string )
{
    {
        image_read_info< png_tag > info = read_image_info( png_filename
                                                         , tag_t() );
        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }

    {
        ifstream in( png_filename.c_str(), ios::binary );

        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }

    {
        FILE* file = fopen( png_filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = read_image_info( file
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_read_image_test )
{
    {
        rgba8_image_t img;
        read_image( png_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        ifstream in( png_filename.c_str(), ios::binary );

        rgba8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        FILE* file = fopen( png_filename.c_str(), "rb" );
        
        rgba8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        typedef bit_aligned_image1_type< 1, gray_layout_t >::type image_t;
        image_t img;

        read_image( png_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

}

BOOST_AUTO_TEST_CASE( png_read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( png_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        rgba8_image_t img;
        read_and_convert_image( png_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        ifstream in( png_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }

    {
        FILE* file = fopen( png_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_and_convert_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 320 );
        BOOST_CHECK_EQUAL( img.height(), 240 );
    }
}

BOOST_AUTO_TEST_CASE( png_read_view_test )
{
    {
        rgba8_image_t img( 320, 240 );
        read_view( png_filename, view( img ), tag_t() );
    }

    {
        ifstream in( png_filename.c_str(), ios::binary );

        rgba8_image_t img( 320, 240 );
        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( png_filename.c_str(), "rb" );
        
        rgba8_image_t img( 320, 240 );
        read_view( file, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( png_read_and_convert_view_test )
{
    {
        rgb8_image_t img( 320, 240 );
        read_and_convert_view( png_filename, view( img ), tag_t() );
    }

    {
        ifstream in( png_filename.c_str(), ios::binary );

        rgb8_image_t img( 320, 240 );
        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( png_filename.c_str(), "rb" );
        
        rgb8_image_t img( 320, 240 );

        read_and_convert_view( file
                             , view( img )
                             , tag_t()
                             );
    }
}

BOOST_AUTO_TEST_CASE( png_write_view_test )
{
    {
        string filename( png_out + "write_test_string.png" );

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), tag_t() );
    }

    {
        string filename( png_out + "write_test_ofstream.png" );

        ofstream out( filename.c_str(), ios::out | ios::binary );

        rgb8_image_t img( 320, 240 );
        write_view( out, view( img ), tag_t() );
    }

    {
        string filename( png_out + "write_test_file.png" );

        FILE* file = fopen( filename.c_str(), "wb" );
        
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), tag_t() );
    }

    {
        string filename( png_out + "write_test_info.png" );
        FILE* file = fopen( filename.c_str(), "wb" );

        image_write_info< png_tag > info;
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), info );
    }
}

BOOST_AUTO_TEST_CASE( png_stream_test )
{
    // 1. Read an image.
    ifstream in( png_filename.c_str(), ios::binary );

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
    string filename( png_out + "stream_test.png" );
    ofstream out( filename.c_str(), ios_base::binary );
    write_view( out, view( dst ), tag_t() );
}
