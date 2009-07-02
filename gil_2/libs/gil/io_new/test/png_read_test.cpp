#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
#define BOOST_GIL_IO_ENABLE_GRAY_ALPHA

#include <boost/cstdint.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/extension/io_new/png_all.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>

#include "cmp_view.hpp"
#include "paths.hpp"

#pragma warning( disable: 4244 ) 

using namespace std;
using namespace boost;
using namespace gil;
using namespace boost::gil::detail;
namespace fs = boost::filesystem;

typedef png_tag tag_t;

BOOST_AUTO_TEST_SUITE( png_test )

typedef pixel< uint8_t, gray_alpha_layout_t > gray_alpha8_pixel_t;
typedef image< gray_alpha8_pixel_t, false   > gray_alpha8_image_t;

typedef pixel< uint16_t, gray_alpha_layout_t > gray_alpha16_pixel_t;
typedef image< gray_alpha16_pixel_t, false   > gray_alpha16_image_t;

template< typename Image >
void test_file( string filename )
{
    Image src, dst;

    image_read_settings< png_tag > settings;
    settings._read_file_gamma        = true;
    settings._read_transparency_data = true;

    image_read_info< png_tag > info = read_image_info( png_in + filename
                                                     , settings
                                                     );

    read_image( png_in + filename
              , src
              , settings
              );

    image_write_info< png_tag > write_info;
    write_info._file_gamma = info._file_gamma;
    
    write_view( png_out + filename
              , view( src )
              , write_info
              );

    read_image( png_out + filename, dst, tag_t() );

    cmp_view( view( src ), view( dst ) );
}

BOOST_AUTO_TEST_CASE( read_header_test )
{
    {
        image_read_info< tag_t > info = read_image_info( png_filename
                                                       , tag_t()
                                                       );

        BOOST_CHECK_EQUAL( info._width , 320 );
        BOOST_CHECK_EQUAL( info._height, 240 );

        BOOST_CHECK_EQUAL( info._num_channels, 4                   );
        BOOST_CHECK_EQUAL( info._bit_depth   , 8                   );
        BOOST_CHECK_EQUAL( info._color_type  , PNG_COLOR_TYPE_RGBA );

        BOOST_CHECK_EQUAL( info._interlace_method  , PNG_INTERLACE_NONE        );
        BOOST_CHECK_EQUAL( info._compression_method, PNG_COMPRESSION_TYPE_BASE );
        BOOST_CHECK_EQUAL( info._filter_method     , PNG_FILTER_TYPE_BASE      );


        BOOST_CHECK_EQUAL( info._file_gamma, 1 );
    }
}

BOOST_AUTO_TEST_CASE( basic_format_test )
{
    // Basic format test files (non-interlaced)

    // basn0g01    -   black & white
    test_file< gray1_image_t >( "basn0g01.png" );

    // basn0g02    -   2 bit (4 level) grayscale
    test_file< gray2_image_t >( "basn0g02.png" );

    // basn0g04    -   4 bit (16 level) grayscale
    test_file< gray4_image_t >( "basn0g04.png" );

    // basn0g08    -   8 bit (256 level) grayscale
    test_file< gray8_image_t >( "basn0g08.png" );

    // basn0g16    -   16 bit (64k level) grayscale
    test_file< gray16_image_t >( "basn0g16.png" );

    // basn2c08    -   3x8 bits rgb color
    test_file< rgb8_image_t >( "basn2c08.png" );

    // basn2c16    -   3x16 bits rgb color
    test_file< rgb16_image_t >( "basn2c16.png" );

    // basn3p01    -   1 bit (2 color) paletted
    test_file< rgb8_image_t >( "basn3p01.png" );

    // basn3p02    -   2 bit (4 color) paletted
    test_file< rgb8_image_t >( "basn3p02.png" );

    // basn3p04    -   4 bit (16 color) paletted
    test_file< rgb8_image_t >( "basn3p04.png" );

    // basn3p08    -   8 bit (256 color) paletted
    test_file< rgb8_image_t >( "basn3p08.png" );

    // basn4a08    -   8 bit grayscale + 8 bit alpha-channel
    test_file< gray_alpha8_image_t >( "basn4a08.png" );

    // basn4a16    -   16 bit grayscale + 16 bit alpha-channel
    test_file< gray_alpha16_image_t >( "basn4a16.png" );

    // basn6a08    -   3x8 bits rgb color + 8 bit alpha-channel
    test_file< rgba8_image_t >( "basn6a08.png" );

    // basn6a16    -   3x16 bits rgb color + 16 bit alpha-channel
    test_file< rgba16_image_t >( "basn6a16.png" );
}

BOOST_AUTO_TEST_CASE( basic_format_interlaced_test )
{
    // Basic format test files (Adam-7 interlaced)

    // basi0g01    -   black & white
    test_file< gray1_image_t >( "basi0g01.png" );

    // basi0g02    -   2 bit (4 level) grayscale
    test_file< gray2_image_t >( "basi0g02.png" );

    // basi0g04    -   4 bit (16 level) grayscale
    test_file< gray4_image_t >( "basi0g04.png" );

    // basi0g08    -   8 bit (256 level) grayscale
    test_file< gray8_image_t >( "basi0g08.png" );

    // basi0g16    -   16 bit (64k level) grayscale
    test_file< gray16_image_t >( "basi0g16.png" );

    // basi2c08    -   3x8 bits rgb color
    test_file< rgb8_image_t >( "basi2c08.png" );

    // basi2c16    -   3x16 bits rgb color
    test_file< rgb16_image_t >( "basi2c16.png" );

    // basi3p01    -   1 bit (2 color) paletted
    test_file< rgb8_image_t >( "basi3p01.png" );

    // basi3p02    -   2 bit (4 color) paletted
    test_file< rgb8_image_t >( "basi3p02.png" );

    // basi3p04    -   4 bit (16 color) paletted
    test_file< rgb8_image_t >( "basi3p04.png" );

    // basi3p08    -   8 bit (256 color) paletted
    test_file< rgb8_image_t >( "basi3p08.png" );

    // basi4a08    -   8 bit grayscale + 8 bit alpha-channel
    test_file< gray_alpha8_image_t >( "basi4a08.png" );

    // basi4a16    -   16 bit grayscale + 16 bit alpha-channel
    test_file< gray_alpha16_image_t >( "basi4a16.png" );

    // basi6a08    -   3x8 bits rgb color + 8 bit alpha-channel
    test_file< rgba8_image_t >( "basi6a08.png" );

    // basi6a16    -   3x16 bits rgb color + 16 bit alpha-channel
    test_file< rgba16_image_t >( "basi6a16.png" );
}

BOOST_AUTO_TEST_CASE( odd_sizes_test )
{
    // s01i3p01 - 1x1 paletted file, interlaced 
    test_file< rgb8_image_t >( "s01i3p01.png" );

    // s01n3p01 - 1x1 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s01n3p01.png" );

    // s02i3p01 - 2x2 paletted file, interlaced 
    test_file< rgb8_image_t >( "s02i3p01.png" );

    // s02n3p01 - 2x2 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s02n3p01.png" );

    // s03i3p01 - 3x3 paletted file, interlaced 
    test_file< rgb8_image_t >( "s03i3p01.png" );

    // s03n3p01 - 3x3 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s03n3p01.png" );

    // s04i3p01 - 4x4 paletted file, interlaced 
    test_file< rgb8_image_t >( "s04i3p01.png" );

    // s04n3p01 - 4x4 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s04n3p01.png" );

    // s05i3p02 - 5x5 paletted file, interlaced 
    test_file< rgb8_image_t >( "s05i3p02.png" );

    // s05n3p02 - 5x5 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s05n3p02.png" );

    // s06i3p02 - 6x6 paletted file, interlaced 
    test_file< rgb8_image_t >( "s06i3p02.png" );

    // s06n3p02 - 6x6 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s06n3p02.png" );

    // s07i3p02 - 7x7 paletted file, interlaced 
    test_file< rgb8_image_t >( "s07i3p02.png" );

    // s07n3p02 - 7x7 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s07n3p02.png" );

    // s08i3p02 - 8x8 paletted file, interlaced 
    test_file< rgb8_image_t >( "s08i3p02.png" );

    // s08n3p02 - 8x8 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s08n3p02.png" );

    // s09i3p02 - 9x9 paletted file, interlaced 
    test_file< rgb8_image_t >( "s09i3p02.png" );

    // s09n3p02 - 9x9 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s09n3p02.png" );

    // s32i3p04 - 32x32 paletted file, interlaced 
    test_file< rgb8_image_t >( "s32i3p04.png" );

    // s32n3p04 - 32x32 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s32n3p04.png" );

    // s33i3p04 - 33x33 paletted file, interlaced 
    test_file< rgb8_image_t >( "s33i3p04.png" );

    // s33n3p04 - 33x33 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s33n3p04.png" );

    // s34i3p04 - 34x34 paletted file, interlaced 
    test_file< rgb8_image_t >( "s34i3p04.png" );

    // s34n3p04 - 34x34 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s34n3p04.png" );

    // s35i3p04 - 35x35 paletted file, interlaced 
    test_file< rgb8_image_t >( "s35i3p04.png" );

    // s35n3p04 - 35x35 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s35n3p04.png" );

    // s36i3p04 - 36x36 paletted file, interlaced 
    test_file< rgb8_image_t >( "s36i3p04.png" );

    // s36n3p04 - 36x36 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s36n3p04.png" );

    // s37i3p04 - 37x37 paletted file, interlaced 
    test_file< rgb8_image_t >( "s37i3p04.png" );

    // s37n3p04 - 37x37 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s37n3p04.png" );

    // s38i3p04 - 38x38 paletted file, interlaced 
    test_file< rgb8_image_t >( "s38i3p04.png" );

    // s38n3p04 - 38x38 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s38n3p04.png" );

    // s39i3p04 - 39x39 paletted file, interlaced 
    test_file< rgb8_image_t >( "s39i3p04.png" );

    // s39n3p04 - 39x39 paletted file, no interlacing 
    test_file< rgb8_image_t >( "s39n3p04.png" );

    // s40i3p04 - 40x40 paletted file, interlaced 
    test_file< rgb8_image_t >( "s40i3p04.png" );

    // s40n3p04 - 40x40 paletted file, no interlacing
    test_file< rgb8_image_t >( "s40n3p04.png" );
}

BOOST_AUTO_TEST_CASE( background_test )
{
    // bgai4a08 - 8 bit grayscale, alpha, no background chunk, interlaced 
    test_file< gray_alpha8_image_t >( "bgai4a08.png" );

    // bgai4a16 - 16 bit grayscale, alpha, no background chunk, interlaced 
    test_file< gray_alpha16_image_t >( "bgai4a16.png" );

    // bgan6a08 - 3x8 bits rgb color, alpha, no background chunk 
    test_file< rgba8_image_t >( "bgan6a08.png" );

    // bgan6a16 - 3x16 bits rgb color, alpha, no background chunk 
    test_file< rgba16_image_t >( "bgan6a16.png" );

    // bgbn4a08 - 8 bit grayscale, alpha, black background chunk 
    test_file< gray_alpha8_image_t >( "bgbn4a08.png" );

    // bggn4a16 - 16 bit grayscale, alpha, gray background chunk 
    test_file< gray_alpha16_image_t >( "bggn4a16.png" );

    // bgwn6a08 - 3x8 bits rgb color, alpha, white background chunk 
    test_file< rgba8_image_t >( "bgwn6a08.png" );

    // bgyn6a16 - 3x16 bits rgb color, alpha, yellow background chunk
    test_file< rgba16_image_t >( "bgyn6a16.png" );
}

BOOST_AUTO_TEST_CASE( transparency_test )
{
    // tbbn1g04 - transparent, black background chunk 
    // file missing
    //test_file< gray_alpha8_image_t >( "tbbn1g04.png" );

    // tbbn2c16 - transparent, blue background chunk 
    test_file< rgba16_image_t >( "tbbn2c16.png" );

    // tbbn3p08 - transparent, black background chunk 
    test_file< rgba8_image_t >( "tbbn3p08.png" );

    // tbgn2c16 - transparent, green background chunk 
    test_file< rgba16_image_t >( "tbgn2c16.png" );

    // tbgn3p08 - transparent, light-gray background chunk 
    test_file< rgba8_image_t >( "tbgn3p08.png" );

    // tbrn2c08 - transparent, red background chunk 
    test_file< rgba8_image_t >( "tbrn2c08.png" );

    // tbwn1g16 - transparent, white background chunk 
    test_file< gray_alpha16_image_t >( "tbwn1g16.png" );

    // tbwn3p08 - transparent, white background chunk 
    test_file< rgba8_image_t >( "tbwn3p08.png" );

    // tbyn3p08 - transparent, yellow background chunk 
    test_file< rgba8_image_t >( "tbyn3p08.png" );

    // tp0n1g08 - not transparent for reference (logo on gray) 
    test_file< gray8_image_t >( "tp0n1g08.png" );

    // tp0n2c08 - not transparent for reference (logo on gray) 
    test_file< rgb8_image_t >( "tp0n2c08.png" );

    // tp0n3p08 - not transparent for reference (logo on gray) 
    test_file< rgb8_image_t >( "tp0n3p08.png" );

    // tp1n3p08 - transparent, but no background chunk
    test_file< rgba8_image_t >( "tp1n3p08.png" );
}

BOOST_AUTO_TEST_CASE( gamma_test )
{
    // g03n0g16 - grayscale, file-gamma = 0.35 
    test_file< gray16_image_t >( "g03n0g16.png" );

    // g03n2c08 - color, file-gamma = 0.35 
    test_file< rgb8_image_t >( "g03n2c08.png" );

    // g03n3p04 - paletted, file-gamma = 0.35 
    test_file< rgb8_image_t >( "g03n3p04.png" );

    // g04n0g16 - grayscale, file-gamma = 0.45 
    test_file< gray16_image_t >( "g04n0g16.png" );

    // g04n2c08 - color, file-gamma = 0.45 
    test_file< rgb8_image_t >( "g04n2c08.png" );

    // g04n3p04 - paletted, file-gamma = 0.45 
    test_file< rgb8_image_t >( "g04n3p04.png" );

    // g05n0g16 - grayscale, file-gamma = 0.55 
    test_file< gray16_image_t >( "g05n0g16.png" );

    // g05n2c08 - color, file-gamma = 0.55 
    test_file< rgb8_image_t >( "g05n2c08.png" );

    // g05n3p04 - paletted, file-gamma = 0.55 
    test_file< rgb8_image_t >( "g05n3p04.png" );

    // g07n0g16 - grayscale, file-gamma = 0.70 
    test_file< gray16_image_t >( "g07n0g16.png" );

    // g07n2c08 - color, file-gamma = 0.70 
    test_file< rgb8_image_t >( "g07n2c08.png" );

    // g07n3p04 - paletted, file-gamma = 0.70 
    test_file< rgb8_image_t >( "g07n3p04.png" );

    // g10n0g16 - grayscale, file-gamma = 1.00 
    test_file< gray16_image_t >( "g10n0g16.png" );

    // g10n2c08 - color, file-gamma = 1.00 
    test_file< rgb8_image_t >( "g10n2c08.png" );

    // g10n3p04 - paletted, file-gamma = 1.00 
    test_file< rgb8_image_t >( "g10n3p04.png" );

    // g25n0g16 - grayscale, file-gamma = 2.50 
    test_file< gray16_image_t >( "g25n0g16.png" );

    // g25n2c08 - color, file-gamma = 2.50 
    test_file< rgb8_image_t >( "g25n2c08.png" );

    // g25n3p04 - paletted, file-gamma = 2.50
    test_file< rgb8_image_t >( "g25n3p04.png" );
}

BOOST_AUTO_TEST_SUITE_END()
