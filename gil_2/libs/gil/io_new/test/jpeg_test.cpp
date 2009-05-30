#include <fstream>

#include <boost/filesystem/path.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost;
using namespace gil;
using namespace filesystem;

typedef jpeg_tag tag_t;

BOOST_AUTO_TEST_SUITE( jpeg_test )

BOOST_AUTO_TEST_CASE( read_image_info_test )
{

    {
        image_read_info< jpeg_tag > info = read_image_info( jpeg_filename
                                                          , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        ifstream in( jpeg_filename.c_str(), ios::binary );

        image_read_info< jpeg_tag > info = read_image_info( in
                                                          , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        FILE* file = fopen( jpeg_filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = boost::gil::read_image_info( file
                                                                   , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        path my_path( jpeg_filename );
        image_read_info< tag_t > info = boost::gil::read_image_info( my_path
                                                                   , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }


}

BOOST_AUTO_TEST_CASE( read_image_test )
{
    {
        rgb8_image_t img;
        read_image( jpeg_filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }

    {
        ifstream in( jpeg_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }

    {
        FILE* file = fopen( jpeg_filename.c_str(), "rb" );
        
        rgb8_image_t img;
        read_image( file, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }

    {
        rgb8_image_t img;

        image_read_settings< jpeg_tag > settings( point_t(  0,  0 )
                                                , point_t( 10, 10 )
                                                , jpeg_dct_method::slow
                                                );

        read_image( jpeg_filename, img, settings );

        BOOST_CHECK_EQUAL( img.width() , 10 );
        BOOST_CHECK_EQUAL( img.height(), 10 );
    }

}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( jpeg_filename, img, tag_t() );
    }

    {
        ifstream in( jpeg_filename.c_str(), ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( read_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        read_view( jpeg_filename, view( img ), tag_t() );
    }

    {
        ifstream in( jpeg_filename.c_str(), ios::binary );

        rgb8_image_t img( 136, 98 );
        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( jpeg_filename.c_str(), "rb" );
        
        rgb8_image_t img( 136, 98 );
        read_view( file, view( img ), tag_t() );
    }
}
BOOST_AUTO_TEST_CASE( read_and_convert_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        read_and_convert_view( jpeg_filename, view( img ), tag_t() );
    }

    {
        ifstream in( jpeg_filename.c_str(), ios::binary );

        rgb8_image_t img( 136, 98 );
        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( jpeg_filename.c_str(), "rb" );

        rgb8_image_t img( 136, 98 );
        read_and_convert_view( file
                             , view( img )
                             , tag_t()
                             );
    }
}

BOOST_AUTO_TEST_CASE( write_view_test )
{
    {
        string filename( jpeg_out + "write_test_string.jpg" );

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), tag_t() );
    }

    {
        string filename( jpeg_out + "write_test_ofstream.jpg" );

        ofstream out( filename.c_str(), ios::binary );

        rgb8_image_t img( 320, 240 );
        write_view( out, view( img ), tag_t() );
    }

    {
        string filename( jpeg_out + "write_test_file.jpg" );

        FILE* file = fopen( filename.c_str(), "wb" );
        
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), tag_t() );
    }

    {
        string filename( jpeg_out + "write_test_info.jpg" );
        FILE* file = fopen( filename.c_str(), "wb" );

        image_write_info< jpeg_tag > info;
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), info );
    }

}

BOOST_AUTO_TEST_CASE( stream_test )
{
    // 1. Read an image.
    ifstream in( jpeg_filename.c_str(), ios::binary );

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
    string filename( jpeg_out + "stream_test.jpg" );
    ofstream out( filename.c_str(), ios_base::binary );
    write_view( out, view( dst ), tag_t() );
}

BOOST_AUTO_TEST_CASE( dynamic_image_test )
{
    typedef mpl::vector< gray8_image_t
                       , gray16_image_t
                       , rgb8_image_t
                       , rgba8_image_t
                       > my_img_types;


    any_image< my_img_types > runtime_image;

    read_image( jpeg_filename.c_str()
              , runtime_image
              , jpeg_tag()
              );

    write_view( jpeg_out + "old_dynamic_image_test.jpg"
              , view( runtime_image )
              , jpeg_tag()
              );
}

BOOST_AUTO_TEST_SUITE_END()
