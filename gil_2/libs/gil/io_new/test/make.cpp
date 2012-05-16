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

        stringstream in_memory( ios_base::in | ios_base::out | ios_base::binary );
        auto backend_istream = make_reader_backend( in_memory, bmp_tag() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto backend_ifstream = make_reader_backend( in, bmp_tag() );

        fs::path my_path( bmp_filename );
        auto backend_path = make_reader_backend( my_path, bmp_tag() );
    }

    {
        auto backend_char   = make_reader_backend( bmp_filename.c_str(), image_read_settings<bmp_tag>() );
        auto backend_string = make_reader_backend( bmp_filename, image_read_settings<bmp_tag>() );

        FILE* file = fopen( bmp_filename.c_str(), "rb" );
        auto backend_file = make_reader_backend( file, image_read_settings<bmp_tag>() );

        stringstream in_memory( ios_base::in | ios_base::out | ios_base::binary );
        auto backend_istream = make_reader_backend( in_memory, image_read_settings<bmp_tag>() );

        ifstream in( bmp_filename.c_str(), ios::binary );
        auto backend_ifstream = make_reader_backend( in, image_read_settings<bmp_tag>() );

        fs::path my_path( bmp_filename );
        auto backend_path = make_reader_backend( my_path, image_read_settings<bmp_tag>() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
