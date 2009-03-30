#include <fstream>

#include <boost/filesystem/path.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#define ADD_FS_PATH_SUPPORT
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;
using namespace boost::filesystem;

typedef jpeg_tag tag_t;

namespace jpeg_test {

extern std::string filename;

BOOST_AUTO_TEST_CASE( read_image_info_test )
{

    {
        image_read_info< jpeg_tag > info = read_image_info( filename
                                                          , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        image_read_info< jpeg_tag > info = read_image_info( in
                                                          , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        FILE* file = fopen( filename.c_str(), "rb" );
        
        image_read_info< tag_t > info = boost::gil::read_image_info( file
                                                                   , tag_t() );

        BOOST_CHECK_EQUAL( info._width , 136 );
        BOOST_CHECK_EQUAL( info._height, 98  );
    }

    {
        path my_path( filename );
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
        read_image( filename, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        rgb8_image_t img;
        read_image( in, img, tag_t() );

        BOOST_CHECK_EQUAL( img.width() , 136 );
        BOOST_CHECK_EQUAL( img.height(), 98 );
    }

    {
        FILE* file = fopen( filename.c_str(), "rb" );
        
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

        read_image( filename, img, settings );

        BOOST_CHECK_EQUAL( img.width() , 10 );
        BOOST_CHECK_EQUAL( img.height(), 10 );
    }

}

BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
{
    {
        rgb8_image_t img;
        read_and_convert_image( filename, img, tag_t() );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        rgb8_image_t img;
        read_and_convert_image( in, img, tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( read_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        read_view( filename, view( img ), tag_t() );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        rgb8_image_t img( 136, 98 );
        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( filename.c_str(), "rb" );
        
        rgb8_image_t img( 136, 98 );
        read_view( file, view( img ), tag_t() );
    }
}
BOOST_AUTO_TEST_CASE( read_and_convert_view_test )
{
    {
        rgb8_image_t img( 136, 98 );
        read_and_convert_view( filename, view( img ), tag_t() );
    }

    {
        ifstream in( filename.c_str(), ios::in | ios::binary );

        rgb8_image_t img( 136, 98 );
        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( filename.c_str(), "rb" );

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
        string filename( jpeg_out + "test1.jpg" );

        gray8_image_t img( 320, 240 );
        write_view( filename, view( img ), tag_t() );
    }

    {
        string filename( jpeg_out + "test2.jpg" );

        ofstream out( filename.c_str(), ios::out | ios::binary );

        rgb8_image_t img( 320, 240 );
        write_view( out, view( img ), tag_t() );
    }

    {
        string filename( jpeg_out + "test3.jpg" );

        FILE* file = fopen( filename.c_str(), "wb" );
        
        rgb8_image_t img( 320, 240 );
        write_view( file, view( img ), tag_t() );
    }
}

} // namespace jpeg_test
