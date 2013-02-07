#include <boost/test/unit_test.hpp>

#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT

#include <fstream>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_SUITE( make_test )

BOOST_AUTO_TEST_CASE( make_reader_backend_test )
{
    {
        auto backend_char   = make_reader_backend( bmp_filename.c_str(), bmp_tag() );
        auto backend_string = make_reader_backend( bmp_filename, bmp_tag() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto backend_file = make_reader_backend( file, bmp_tag() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto backend_ifstream = make_reader_backend( in, bmp_tag() );

        fs::path my_path( bmp_filename );
        auto backend_wstring = make_reader_backend( my_path.wstring(), bmp_tag() );
        auto backend_path    = make_reader_backend( my_path          , bmp_tag() );
    }

    {
        auto backend_char   = make_reader_backend( bmp_filename.c_str(), image_read_settings<bmp_tag>() );
        auto backend_string = make_reader_backend( bmp_filename, image_read_settings<bmp_tag>() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto backend_file = make_reader_backend( file, image_read_settings<bmp_tag>() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto backend_ifstream = make_reader_backend( in, image_read_settings<bmp_tag>() );

        fs::path my_path( bmp_filename );
        auto backend_wstring = make_reader_backend( my_path.wstring(), image_read_settings<bmp_tag>() );
        auto backend_path    = make_reader_backend( my_path          , image_read_settings<bmp_tag>() );
    }
}

BOOST_AUTO_TEST_CASE( make_reader_test )
{
    {
        auto reader_char   = make_reader( bmp_filename.c_str(), bmp_tag(), detail::read_and_no_convert() );
        auto reader_string = make_reader( bmp_filename, bmp_tag(), detail::read_and_no_convert() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto reader_file = make_reader( file, bmp_tag(), detail::read_and_no_convert() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto reader_ifstream = make_reader( in, bmp_tag(), detail::read_and_no_convert() );

        fs::path my_path( bmp_filename );
        auto reader_wstring = make_reader( my_path.wstring(), bmp_tag(), detail::read_and_no_convert() );
        auto reader_path    = make_reader( my_path          , bmp_tag(), detail::read_and_no_convert() );
    }

    {
        auto reader_char   = make_reader( bmp_filename.c_str(), image_read_settings< bmp_tag >(), detail::read_and_no_convert() );
        auto reader_string = make_reader( bmp_filename, image_read_settings< bmp_tag >(), detail::read_and_no_convert() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto reader_file = make_reader( file, image_read_settings< bmp_tag >(), detail::read_and_no_convert() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto reader_ifstream = make_reader( in, image_read_settings< bmp_tag >(), detail::read_and_no_convert() );

        fs::path my_path( bmp_filename );
        auto reader_wstring = make_reader( my_path.wstring(), image_read_settings< bmp_tag >(), detail::read_and_no_convert() );
        auto reader_path    = make_reader( my_path          , image_read_settings< bmp_tag >(), detail::read_and_no_convert() );
    }
}

BOOST_AUTO_TEST_CASE( make_dynamic_image_reader_test )
{
    {
        auto reader_char   = make_dynamic_image_reader( bmp_filename.c_str(), bmp_tag() );
        auto reader_string = make_dynamic_image_reader( bmp_filename, bmp_tag() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto reader_file = make_dynamic_image_reader( file, bmp_tag() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto reader_ifstream = make_dynamic_image_reader( in, bmp_tag() );

        fs::path my_path( bmp_filename );
        auto reader_wstring = make_dynamic_image_reader( my_path.wstring(), bmp_tag() );
        auto reader_path    = make_dynamic_image_reader( my_path          , bmp_tag() );
    }

    {
        auto reader_char   = make_dynamic_image_reader( bmp_filename.c_str(), image_read_settings< bmp_tag >() );
        auto reader_string = make_dynamic_image_reader( bmp_filename, image_read_settings< bmp_tag >() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto reader_file = make_dynamic_image_reader( file, image_read_settings< bmp_tag >() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto reader_ifstream = make_dynamic_image_reader( in, image_read_settings< bmp_tag >() );

        fs::path my_path( bmp_filename );
        auto reader_wstring = make_dynamic_image_reader( my_path.wstring(), image_read_settings< bmp_tag >() );
        auto reader_path    = make_dynamic_image_reader( my_path          , image_read_settings< bmp_tag >() );
    }
}

BOOST_AUTO_TEST_CASE( make_writer_test )
{

    {
        //const char file_name[] = "c:\\chhenning\\rgb.bmp";

        //typedef get_write_device< const char*
        //                        , bmp_tag
        //                        >::type write_device_t;
        //
        //write_device_t f( file_name, detail::file_stream_device< bmp_tag >::write_tag() );

        //typedef get_writer< const char*
        //                  , bmp_tag
        //                  >::type writer_t;

        //writer_t w( bmp_filename.c_str(), image_write_info< bmp_tag >() );

        //BOOST_STATIC_ASSERT(( is_same< detail::is_writer< writer_t >::type, boost::mpl::true_ >::value ));
    }

    {
        typedef get_writer< const char*
                          , bmp_tag
                          >::type writer_t;

        BOOST_STATIC_ASSERT(( boost::is_same< detail::is_writer< writer_t >::type, boost::mpl::true_ >::value ));
    }

    {
        auto writer_char   = make_writer(( bmp_out + "make_test.bmp" ).c_str(), bmp_tag() );
        auto writer_string = make_writer(( bmp_out + "make_test.bmp" ), bmp_tag() );

        FILE* file = fopen(( bmp_out + "make_test.bmp" ).c_str(), "wb" );
        auto writer_file = make_writer( file, bmp_tag() );

        ofstream out(( bmp_out  + "make_test.bmp" ).c_str(), ios::binary );
        auto writer_ofstream = make_writer( out, bmp_tag() );

        fs::path my_path( ( bmp_out  + "make_test.bmp" ).c_str() );
        auto writer_wstring = make_writer( my_path.wstring(), bmp_tag() );
        auto writer_path    = make_writer( my_path          , bmp_tag() );
    }

    {
        auto writer_char   = make_writer(( bmp_out + "make_test.bmp" ).c_str(), image_write_info< bmp_tag >() );
        auto writer_string = make_writer(( bmp_out + "make_test.bmp" )        , image_write_info< bmp_tag >() );

        FILE* file = fopen( (bmp_out + string( "make_test.bmp")).c_str(), "wb" );
        auto writer_file = make_writer( file, image_write_info< bmp_tag >() );

        ofstream out( ( bmp_out  + "make_test.bmp" ).c_str(), ios::binary );
        auto writer_ofstream = make_writer( out, image_write_info< bmp_tag >() );

        fs::path my_path( bmp_out  + "make_test.bmp" );
        auto writer_wstring = make_writer( my_path.wstring(), image_write_info< bmp_tag >() );
        auto writer_path    = make_writer( my_path          , image_write_info< bmp_tag >() );
    }
}

BOOST_AUTO_TEST_CASE( make_dynamic_image_writer_test )
{
    {
        auto writer_char   = make_dynamic_image_writer( (bmp_out + string( "make_test.bmp")).c_str(), bmp_tag() );
        auto writer_string = make_dynamic_image_writer( bmp_out  + "make_test.bmp", bmp_tag() );

        FILE* file = fopen( (bmp_out + string( "make_test.bmp")).c_str(), "wb" );
        auto writer_file = make_dynamic_image_writer( file, bmp_tag() );

        ofstream out(( bmp_out  + "make_test.bmp" ).c_str(), ios::binary );
        auto writer_ofstream = make_dynamic_image_writer( out, bmp_tag() );

        fs::path my_path( bmp_out  + "make_test.bmp" );
        auto writer_wstring = make_dynamic_image_writer( my_path.wstring(), bmp_tag() );
        auto writer_path    = make_dynamic_image_writer( my_path          , bmp_tag() );
    }

    {
        auto writer_char   = make_dynamic_image_writer( (bmp_out + string( "make_test.bmp")).c_str(), image_write_info< bmp_tag >() );
        auto writer_string = make_dynamic_image_writer( bmp_out  + "make_test.bmp", image_write_info< bmp_tag >() );

        FILE* file = fopen( (bmp_out + string( "make_test.bmp")).c_str(), "wb" );
        auto writer_file = make_dynamic_image_writer( file, image_write_info< bmp_tag >() );

        ofstream out(( bmp_out  + "make_test.bmp" ).c_str(), ios::binary );
        auto writer_ofstream = make_dynamic_image_writer( out, image_write_info< bmp_tag >() );

        fs::path my_path( bmp_out  + "make_test.bmp" );
        auto writer_wstring = make_dynamic_image_writer( my_path.wstring(), image_write_info< bmp_tag >() );
        auto writer_path    = make_dynamic_image_writer( my_path          , image_write_info< bmp_tag >() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
