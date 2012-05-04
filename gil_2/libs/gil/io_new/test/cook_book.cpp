
#include <fstream>

#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem/convenience.hpp>


#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>
#include <boost/gil/extension/io_new/targa_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_SUITE( cook_book )

template< typename T
        , typename FormatTag
        , typename Device
        >
void test_get_read_device()
{
    BOOST_STATIC_ASSERT(( boost::is_same< Device, get_read_device< T, FormatTag >::type >::value ));
}

template< typename T
        , typename FormatTag
        , typename Device
        >
void test_get_write_device()
{
    BOOST_STATIC_ASSERT(( boost::is_same< Device, get_write_device< T, FormatTag >::type >::value ));
}


BOOST_AUTO_TEST_CASE( recipe_1 )
{
    typedef bmp_tag tag_t;

    rgb8_image_t img;


    typedef get_reader< const std::string
                      , tag_t
                      >::type reader_t;


    reader_t reader = make_reader< const std::string
                                 , tag_t
                                 , detail::read_and_no_convert
                                 >
                                 ( bmp_filename
                                 , tag_t()
                                 );

    reader.init_image( img
                     , reader._settings
                     );


    reader.apply( view( img ));
}

BOOST_AUTO_TEST_CASE( recipe_2 )
{
    typedef tiff_tag tag_t;

    {
        typedef get_reader_backend< const char*
                                  , tag_t
                                  >::type backend_t;

        backend_t a = read_image_info( bmp_filename.c_str(), tag_t() );
        backend_t b = read_image_info( bmp_filename.c_str(), image_read_settings<tag_t>() );
    }

    //{
    //    typedef get_reader_backend< const std::string
    //                              , tag_t
    //                              >::type backend_t;

    //    backend_t a = read_image_info( bmp_filename, tag_t() );
    //    backend_t b = read_image_info( bmp_filename, image_read_settings<tag_t>() );
    //}

    //{
    //    std::ifstream in( bmp_filename.c_str(), ios::binary );

    //    typedef get_reader_backend< std::ifstream
    //                              , tag_t
    //                              >::type backend_t;

    //    backend_t a = read_image_info( in, tag_t() );
    //    backend_t b = read_image_info( in, image_read_settings<tag_t>() );
    //}

    //{
    //    FILE* file = fopen( bmp_filename.c_str(), "rb" );

    //    typedef get_reader_backend< FILE*
    //                              , tag_t
    //                              >::type backend_t;

    //    backend_t a = read_image_info( file, tag_t() );
    //    backend_t b = read_image_info( file, image_read_settings<tag_t>() );
    //}

    //{
    //    fs::path my_path( bmp_filename );

    //    typedef get_reader_backend< fs::path
    //                              , tag_t
    //                              >::type backend_t;

    //    backend_t a = read_image_info( my_path, tag_t() );
    //    backend_t b = read_image_info( my_path, image_read_settings<tag_t>() );
    //}
}

BOOST_AUTO_TEST_CASE( recipe_3 )
{
    typedef bmp_tag tag_t;

    {
        rgb8_image_t img;

        read_image( bmp_filename.c_str(), img, tag_t() );
    }

    {
        rgb8_image_t img;

        read_image( bmp_filename, img, tag_t() );
    }

    {
        std::ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;

        read_image( in, img, tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );

        rgb8_image_t img;

        read_image( file, img, tag_t() );
    }

    {
        fs::path my_path( bmp_filename );

        rgb8_image_t img;

        read_image( my_path, img, tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( recipe_4 )
{
    typedef bmp_tag tag_t;

    {
        rgb8_image_t img;

        read_view( bmp_filename.c_str(), view( img ), tag_t() );
    }

    {
        rgb8_image_t img;

        read_view( bmp_filename, view( img ), tag_t() );
    }

    {
        std::ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;

        read_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );

        rgb8_image_t img;

        read_view( file, view( img ), tag_t() );
    }

    {
        fs::path my_path( bmp_filename );

        rgb8_image_t img;

        read_view( my_path, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( recipe_5 )
{
    typedef bmp_tag tag_t;

    {
        rgb8_image_t img;

        read_and_convert_image( bmp_filename.c_str(), img, tag_t() );
    }

    {
        rgb8_image_t img;

        read_and_convert_image( bmp_filename, img, tag_t() );
    }

    {
        std::ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;

        read_and_convert_image( in, img, tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );

        rgb8_image_t img;

        read_and_convert_image( file, img, tag_t() );
    }

    {
        fs::path my_path( bmp_filename );

        rgb8_image_t img;

        read_and_convert_image( my_path, img, tag_t() );
    }
}

BOOST_AUTO_TEST_CASE( recipe_6 )
{
    typedef bmp_tag tag_t;

    {
        rgb8_image_t img;

        read_and_convert_view( bmp_filename.c_str(), view( img ), tag_t() );
    }

    {
        rgb8_image_t img;

        read_and_convert_view( bmp_filename, view( img ), tag_t() );
    }

    {
        std::ifstream in( bmp_filename.c_str(), ios::binary );

        rgb8_image_t img;

        read_and_convert_view( in, view( img ), tag_t() );
    }

    {
        FILE* file = fopen( bmp_filename.c_str(), "rb" );

        rgb8_image_t img;

        read_and_convert_view( file, view( img ), tag_t() );
    }

    {
        fs::path my_path( bmp_filename );

        rgb8_image_t img;

        read_and_convert_view( my_path, view( img ), tag_t() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
