//#define BOOST_TEST_MODULE raw_test_module
#include <boost/test/unit_test.hpp>

#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT

#include <fstream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/raw_all.hpp>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem/convenience.hpp>

#include "mandel_view.hpp"
#include "paths.hpp"
#include "subimage_test.hpp"

using namespace std;
using namespace boost;
using namespace gil;
namespace fs = boost::filesystem;

typedef raw_tag tag_t;

BOOST_AUTO_TEST_SUITE( raw_test )

BOOST_AUTO_TEST_CASE( read_image_info_using_string )
{

    {
        get_read_device< char*, tag_t >::type l;

        //typedef get_reader_backend< const std::string
        //                          , tag_t
        //                          >::type backend_t;

        //backend_t backend = read_image_info( raw_filename
        //                                   , tag_t()
        //                                   );

        //BOOST_CHECK_EQUAL( backend._info._width , 127 );
        //BOOST_CHECK_EQUAL( backend._info._height, 64 );
    }

}

//    {
//        ifstream in( raw_filename.c_str(), ios::binary );
//
//        typedef get_reader_backend< std::ifstream
//                                  , tag_t
//                                  >::type backend_t;
//
//        backend_t backend = read_image_info( in
//                                           , tag_t()
//                                           );
//
//        BOOST_CHECK_EQUAL( backend._info._width , 127 );
//        BOOST_CHECK_EQUAL( backend._info._height, 64 );
//    }
//
//    {
//        FILE* file = fopen( raw_filename.c_str(), "rb" );
//
//        typedef get_reader_backend< FILE*
//                                  , tag_t
//                                  >::type backend_t;
//
//        backend_t backend = read_image_info( file
//                                           , tag_t()
//                                           );
//        
//        BOOST_CHECK_EQUAL( backend._info._width , 127 );
//        BOOST_CHECK_EQUAL( backend._info._height, 64 );
//    }
//
//    {
//        fs::path my_path( raw_filename );
//
//        typedef get_reader_backend< fs::path
//                                  , tag_t
//                                  >::type backend_t;
//
//        backend_t backend = read_image_info( my_path
//                                           , tag_t()
//                                           );
//
//        BOOST_CHECK_EQUAL( backend._info._width , 127 );
//        BOOST_CHECK_EQUAL( backend._info._height, 64  );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( read_image_test )
//{
//    {
//        rgb8_image_t img;
//        read_image( raw_filename, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//
//    {
//        ifstream in( raw_filename.c_str(), ios::binary );
//
//        rgb8_image_t img;
//        read_image( in, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//
//    {
//        FILE* file = fopen( raw_filename.c_str(), "rb" );
//        
//        rgb8_image_t img;
//        read_image( file, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//
//    {
//        fs::path my_path( raw_filename );
//
//        rgb8_image_t img;
//        read_image( my_path, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( read_and_convert_image_test )
//{
//    {
//        rgb8_image_t img;
//        read_and_convert_image( raw_filename, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//
//    {
//        ifstream in( raw_filename.c_str(), ios::binary );
//
//        rgb8_image_t img;
//        read_and_convert_image( in, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//
//    {
//        FILE* file = fopen( raw_filename.c_str(), "rb" );
//        
//        rgb8_image_t img;
//        read_and_convert_image( file, img, tag_t() );
//
//        BOOST_CHECK_EQUAL( img.width() , 127 );
//        BOOST_CHECK_EQUAL( img.height(), 64 );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( read_view_test )
//{
//    {
//        rgb8_image_t img( 127, 64 );
//        read_view( raw_filename, view( img ), tag_t() );
//    }
//
//    {
//        ifstream in( raw_filename.c_str(), ios::binary );
//
//        rgb8_image_t img( 127, 64 );
//        read_view( in, view( img ), tag_t() );
//    }
//
//    {
//        FILE* file = fopen( raw_filename.c_str(), "rb" );
//        
//        rgb8_image_t img( 127, 64 );
//        read_view( file, view( img ), tag_t() );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( read_and_convert_view_test )
//{
//    {
//        rgb8_image_t img( 127, 64 );
//        read_and_convert_view( raw_filename, view( img ), tag_t() );
//    }
//
//    {
//        ifstream in( raw_filename.c_str(), ios::binary );
//
//        rgb8_image_t img( 127, 64 );
//        read_and_convert_view( in, view( img ), tag_t() );
//    }
//
//    {
//        FILE* file = fopen( raw_filename.c_str(), "rb" );
//        
//        rgb8_image_t img( 127, 64 );
//        read_and_convert_view( file
//                             , view( img )
//                             , tag_t()
//                             );
//    }
//}
//
//BOOST_AUTO_TEST_CASE( stream_test )
//{
//    // 1. Read an image.
//    ifstream in( raw_filename.c_str(), ios::binary );
//
//    rgb8_image_t img;
//    read_image( in, img, tag_t() );
//
//    // no raw write available
//}
//
//BOOST_AUTO_TEST_CASE( stream_test_2 )
//{
//    filebuf in_buf;
//    if( !in_buf.open( raw_filename.c_str(), ios::in | ios::binary ) )
//    {
//        BOOST_CHECK( false );
//    }
//
//    istream in( &in_buf );
//
//    rgb8_image_t img;
//    read_image( in, img, tag_t() );
//}
//
//BOOST_AUTO_TEST_CASE( subimage_test )
//{
//    run_subimage_test< rgb8_image_t, tag_t >( raw_filename
//                                            , point_t(   0, 0 )
//                                            , point_t( 127, 1 )
//                                            );
//
//    run_subimage_test< rgb8_image_t, tag_t >( raw_filename
//                                            , point_t( 39,  7 )
//                                            , point_t( 50, 50 )
//                                            );
//}
//
//BOOST_AUTO_TEST_CASE( dynamic_image_test )
//{
//    typedef mpl::vector< gray8_image_t
//                       , gray16_image_t
//                       , rgb8_image_t
//                       , rgba8_image_t
//                       > my_img_types;
//
//
//    any_image< my_img_types > runtime_image;
//
//    read_image( raw_filename.c_str()
//              , runtime_image
//              , tag_t()
//              );
//}

BOOST_AUTO_TEST_SUITE_END()
