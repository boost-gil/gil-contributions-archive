
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

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef tiff_tag tag_t;

BOOST_AUTO_TEST_SUITE( tiff_test )

BOOST_AUTO_TEST_CASE( read_image_info_test )
{
    {
        image_read_info< tag_t > info = read_image_info( tiff_filename
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 133 );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );
        image_read_info< tag_t > info = read_image_info( in
                                                       , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 133 );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        image_read_info< tag_t > info = read_image_info( file
                                                       , tag_t() );
        
        BOOST_CHECK_EQUAL( info._width , 200 );
        BOOST_CHECK_EQUAL( info._height, 133 );
    }
}

BOOST_AUTO_TEST_CASE( read_image_test )
{
    {
        rgb8_image_t img;
        read_image( tiff_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }

    {

        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }
}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( tiff_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }

    {
        ifstream in( tiff_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
    }

    {
        TIFF* file = TIFFOpen( tiff_filename.c_str(), "r" );

        rgb8_image_t img;
        read_and_convert_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
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

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
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

        BOOST_CHECK_EQUAL( img.width() , 200 );
        BOOST_CHECK_EQUAL( img.height(), 133 );
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

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), tiff_tag() );
    }

    {
        string filename( tiff_out + "write_test_ofstream.tif" );

        ofstream out( filename.c_str(), ios_base::binary );
        write_view( out, view( img ), tag_t() );
    }

    {
        string filename( tiff_out + "write_test_tiff.tif" );

        TIFF* file = TIFFOpen( filename.c_str(), "w" );
        
        write_view( file, view( img ), tag_t() );
    }

    {
        string filename( tiff_out + "write_test_info.tif" );

        image_write_info< tiff_tag > info;
        write_view( filename, view( img ), info );
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

BOOST_AUTO_TEST_SUITE_END()
