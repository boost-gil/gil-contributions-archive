#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
#define BOOST_GIL_IO_ENABLE_GRAY_ALPHA

#include <boost/cstdint.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/extension/io_new/png_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

#pragma warning( disable: 4244 ) 

using namespace std;
using namespace boost;
using namespace gil;
using namespace boost::gil::detail;
namespace fs = boost::filesystem;

typedef png_tag tag_t;

BOOST_AUTO_TEST_SUITE( png_test )

template< typename Image >
void write( Image&        img
          , const string& file_name
          )
{
    write_view( png_out + file_name
              , view( img )
              , tag_t()
              );
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


        BOOST_CHECK_EQUAL( info._gamma, 0 );

        BOOST_CHECK_EQUAL( info._sig_bits, png_color_8p( NULL ) );
    }
}

BOOST_AUTO_TEST_CASE( basic_format_test )
{
    // Basic format test files (non-interlaced)

    // basn0g01    -   black & white
    {
        gray1_image_t img;

        read_image( png_in + "basn0g01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn0g01.png" );
    }

    // basn0g02    -   2 bit (4 level) grayscale
    {
        gray2_image_t img;

        read_image( png_in + "basn0g02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn0g02.png" );
    }

    // basn0g04    -   4 bit (16 level) grayscale
    {
        gray4_image_t img;

        read_image( png_in + "basn0g04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn0g04.png" );
    }

    // basn0g08    -   8 bit (256 level) grayscale
    {
        gray8_image_t img;

        read_image( png_in + "basn0g08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn0g08.png" );
    }

    // basn0g16    -   16 bit (64k level) grayscale
    {
        gray16_image_t img;

        read_image( png_in + "basn0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn0g16.png" );
    }

    // basn2c08    -   3x8 bits rgb color
    {
        rgb8_image_t img;

        read_image( png_in + "basn2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn2c08.png" );
    }

    // basn2c16    -   3x16 bits rgb color
    {
        rgb16_image_t img;

        read_image( png_in + "basn2c16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn2c16.png" );
    }

    // basn3p01    -   1 bit (2 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basn3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn3p01.png" );
    }

    // basn3p02    -   2 bit (4 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basn3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn3p02.png" );
    }

    // basn3p04    -   4 bit (16 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basn3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn3p04.png" );
    }

    // basn3p08    -   8 bit (256 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basn3p08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn3p08.png" );
    }

    // basn4a08    -   8 bit grayscale + 8 bit alpha-channel
    {
        typedef pixel< uint8_t, gray_alpha_layout_t > gray_alpha8_pixel_t;
        typedef image< gray_alpha8_pixel_t, false   > gray_alpha8_image_t;

        gray_alpha8_image_t img;

        read_image( png_in + "basn4a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn4a08.png" );
    }

    // basn4a16    -   16 bit grayscale + 16 bit alpha-channel
    {
        typedef pixel< uint16_t, gray_alpha_layout_t > gray_alpha16_pixel_t;
        typedef image< gray_alpha16_pixel_t, false   > gray_alpha16_image_t;

        gray_alpha16_image_t img;

        read_image( png_in + "basn4a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn4a16.png" );
    }

    // basn6a08    -   3x8 bits rgb color + 8 bit alpha-channel
    {
        rgba8_image_t img;

        read_image( png_in + "basn6a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn6a08.png" );
    }

    // basn6a16    -   3x16 bits rgb color + 16 bit alpha-channel
    {
        rgba16_image_t img;

        read_image( png_in + "basn6a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basn6a16.png" );
    }
}

BOOST_AUTO_TEST_CASE( basic_format_interlaced_test )
{
    // Basic format test files (Adam-7 interlaced)

    // basi0g01    -   black & white
    {
        gray1_image_t img;

        read_image( png_in + "basi0g01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi0g01.png" );
    }

    // basi0g02    -   2 bit (4 level) grayscale
    {
        gray2_image_t img;

        read_image( png_in + "basi0g02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi0g02.png" );
    }

    // basi0g04    -   4 bit (16 level) grayscale
    {
        gray4_image_t img;

        read_image( png_in + "basi0g04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi0g04.png" );
    }

    // basi0g08    -   8 bit (256 level) grayscale
    {
        gray8_image_t img;

        read_image( png_in + "basi0g08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi0g08.png" );
    }

    // basi0g16    -   16 bit (64k level) grayscale
    {
        gray16_image_t img;

        read_image( png_in + "basi0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi0g16.png" );
    }

    // basi2c08    -   3x8 bits rgb color
    {
        rgb8_image_t img;

        read_image( png_in + "basi2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi2c08.png" );
    }

    // basi2c16    -   3x16 bits rgb color
    {
        rgb16_image_t img;

        read_image( png_in + "basi2c16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi2c16.png" );
    }

    // basi3p01    -   1 bit (2 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basi3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi3p01.png" );
    }

    // basi3p02    -   2 bit (4 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basi3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi3p02.png" );
    }

    // basi3p04    -   4 bit (16 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basi3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi3p04.png" );
    }

    // basi3p08    -   8 bit (256 color) paletted
    {
        rgb8_image_t img;

        read_image( png_in + "basi3p08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi3p08.png" );
    }

    // basi4a08    -   8 bit grayscale + 8 bit alpha-channel
    {
        typedef pixel< uint8_t, gray_alpha_layout_t > gray_alpha8_pixel_t;
        typedef image< gray_alpha8_pixel_t, false   > gray_alpha8_image_t;

        gray_alpha8_image_t img;

        read_image( png_in + "basi4a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi4a08.png" );
    }

    // basi4a16    -   16 bit grayscale + 16 bit alpha-channel
    {
        typedef pixel< uint16_t, gray_alpha_layout_t > gray_alpha16_pixel_t;
        typedef image< gray_alpha16_pixel_t, false   > gray_alpha16_image_t;

        gray_alpha16_image_t img;

        read_image( png_in + "basi4a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi4a16.png" );
    }

    // basi6a08    -   3x8 bits rgb color + 8 bit alpha-channel
    {
        rgba8_image_t img;

        read_image( png_in + "basi6a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi6a08.png" );
    }

    // basi6a16    -   3x16 bits rgb color + 16 bit alpha-channel
    {
        rgba16_image_t img;

        read_image( png_in + "basi6a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "basi6a16.png" );
    }

}

BOOST_AUTO_TEST_CASE( odd_sizes_test )
{
    // s01i3p01 - 1x1 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s01i3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 1 );
        BOOST_CHECK_EQUAL( view( img ).height(), 1 );

        write( img, "s01i3p01.png" );
    }

    // s01n3p01 - 1x1 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s01n3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 1 );
        BOOST_CHECK_EQUAL( view( img ).height(), 1 );

        write( img, "s01n3p01.png" );
    }

    // s02i3p01 - 2x2 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s02i3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 2 );
        BOOST_CHECK_EQUAL( view( img ).height(), 2 );

        write( img, "s02i3p01.png" );
    }

    // s02n3p01 - 2x2 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s02n3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 2 );
        BOOST_CHECK_EQUAL( view( img ).height(), 2 );

        write( img, "s02n3p01.png" );
    }

    // s03i3p01 - 3x3 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s03i3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 3 );
        BOOST_CHECK_EQUAL( view( img ).height(), 3 );

        write( img, "s03i3p01.png" );
    }

    // s03n3p01 - 3x3 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s03n3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 3 );
        BOOST_CHECK_EQUAL( view( img ).height(), 3 );

        write( img, "s03n3p01.png" );
    }

    // s04i3p01 - 4x4 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s04i3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 4 );
        BOOST_CHECK_EQUAL( view( img ).height(), 4 );

        write( img, "s04i3p01.png" );
    }

    // s04n3p01 - 4x4 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s04n3p01.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 4 );
        BOOST_CHECK_EQUAL( view( img ).height(), 4 );

        write( img, "s04n3p01.png" );
    }

    // s05i3p02 - 5x5 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s05i3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 5 );
        BOOST_CHECK_EQUAL( view( img ).height(), 5 );

        write( img, "s05i3p02.png" );
    }

    // s05n3p02 - 5x5 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s05n3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 5 );
        BOOST_CHECK_EQUAL( view( img ).height(), 5 );

        write( img, "s05n3p02.png" );
    }

    // s06i3p02 - 6x6 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s06i3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 6 );
        BOOST_CHECK_EQUAL( view( img ).height(), 6 );

        write( img, "s06i3p02.png" );
    }

    // s06n3p02 - 6x6 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s06n3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 6 );
        BOOST_CHECK_EQUAL( view( img ).height(), 6 );

        write( img, "s06n3p02.png" );
    }

    // s07i3p02 - 7x7 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s07i3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 7 );
        BOOST_CHECK_EQUAL( view( img ).height(), 7 );

        write( img, "s07i3p02.png" );
    }

    // s07n3p02 - 7x7 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s07n3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 7 );
        BOOST_CHECK_EQUAL( view( img ).height(), 7 );

        write( img, "s07n3p02.png" );
    }

    // s08i3p02 - 8x8 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s08i3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 8 );
        BOOST_CHECK_EQUAL( view( img ).height(), 8 );

        write( img, "s08i3p02.png" );
    }

    // s08n3p02 - 8x8 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s08n3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 8 );
        BOOST_CHECK_EQUAL( view( img ).height(), 8 );

        write( img, "s08n3p02.png" );
    }

    // s09i3p02 - 9x9 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s09i3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 9 );
        BOOST_CHECK_EQUAL( view( img ).height(), 9 );

        write( img, "s09i3p02.png" );
    }

    // s09n3p02 - 9x9 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s09n3p02.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 9 );
        BOOST_CHECK_EQUAL( view( img ).height(), 9 );

        write( img, "s09n3p02.png" );
    }

    // s32i3p04 - 32x32 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s32i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "s32i3p04.png" );
    }

    // s32n3p04 - 32x32 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s32n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "s32n3p04.png" );
    }

    // s33i3p04 - 33x33 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s33i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 33 );
        BOOST_CHECK_EQUAL( view( img ).height(), 33 );

        write( img, "s33i3p04.png" );
    }

    // s33n3p04 - 33x33 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s33n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 33 );
        BOOST_CHECK_EQUAL( view( img ).height(), 33 );

        write( img, "s33n3p04.png" );
    }

    // s34i3p04 - 34x34 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s34i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 34 );
        BOOST_CHECK_EQUAL( view( img ).height(), 34 );

        write( img, "s34i3p04.png" );
    }

    // s34n3p04 - 34x34 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s34n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 34 );
        BOOST_CHECK_EQUAL( view( img ).height(), 34 );

        write( img, "s34n3p04.png" );
    }

    // s35i3p04 - 35x35 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s35i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 35 );
        BOOST_CHECK_EQUAL( view( img ).height(), 35 );

        write( img, "s35i3p04.png" );
    }

    // s35n3p04 - 35x35 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s35n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 35 );
        BOOST_CHECK_EQUAL( view( img ).height(), 35 );

        write( img, "s35n3p04.png" );
    }

    // s36i3p04 - 36x36 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s36i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 36 );
        BOOST_CHECK_EQUAL( view( img ).height(), 36 );

        write( img, "s36i3p04.png" );
    }

    // s36n3p04 - 36x36 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s36n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 36 );
        BOOST_CHECK_EQUAL( view( img ).height(), 36 );

        write( img, "s36n3p04.png" );
    }

    // s37i3p04 - 37x37 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s37i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 37 );
        BOOST_CHECK_EQUAL( view( img ).height(), 37 );

        write( img, "s37i3p04.png" );
    }

    // s37n3p04 - 37x37 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s37n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 37 );
        BOOST_CHECK_EQUAL( view( img ).height(), 37 );

        write( img, "s37n3p04.png" );
    }

    // s38i3p04 - 38x38 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s38i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 38 );
        BOOST_CHECK_EQUAL( view( img ).height(), 38 );

        write( img, "s38i3p04.png" );
    }

    // s38n3p04 - 38x38 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s38n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 38 );
        BOOST_CHECK_EQUAL( view( img ).height(), 38 );

        write( img, "s38n3p04.png" );
    }

    // s39i3p04 - 39x39 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s39i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 39 );
        BOOST_CHECK_EQUAL( view( img ).height(), 39 );

        write( img, "s39i3p04.png" );
    }

    // s39n3p04 - 39x39 paletted file, no interlacing 
    {
        rgb8_image_t img;

        read_image( png_in + "s39n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 39 );
        BOOST_CHECK_EQUAL( view( img ).height(), 39 );

        write( img, "s39n3p04.png" );
    }

    // s40i3p04 - 40x40 paletted file, interlaced 
    {
        rgb8_image_t img;

        read_image( png_in + "s40i3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 40 );
        BOOST_CHECK_EQUAL( view( img ).height(), 40 );

        write( img, "s40i3p04.png" );
    }

    // s40n3p04 - 40x40 paletted file, no interlacing
    {
        rgb8_image_t img;

        read_image( png_in + "s40n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 40 );
        BOOST_CHECK_EQUAL( view( img ).height(), 40 );

        write( img, "s40n3p04.png" );
    }
}

BOOST_AUTO_TEST_CASE( background_test )
{
    // bgai4a08 - 8 bit grayscale, alpha, no background chunk, interlaced 
    {
        gray_alpha8_image_t img;

        read_image( png_in + "bgai4a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgai4a08.png" );
    }

    // bgai4a16 - 16 bit grayscale, alpha, no background chunk, interlaced 
    {
        gray_alpha16_image_t img;

        read_image( png_in + "bgai4a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgai4a16.png" );
    }

    // bgan6a08 - 3x8 bits rgb color, alpha, no background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "bgan6a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgan6a08.png" );
    }

    // bgan6a16 - 3x16 bits rgb color, alpha, no background chunk 
    {
        rgba16_image_t img;

        read_image( png_in + "bgan6a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgan6a16.png" );
    }

    // bgbn4a08 - 8 bit grayscale, alpha, black background chunk 
    {
        gray_alpha8_image_t img;

        read_image( png_in + "bgbn4a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgbn4a08.png" );
    }

    // bggn4a16 - 16 bit grayscale, alpha, gray background chunk 
    {
        gray_alpha16_image_t img;

        read_image( png_in + "bggn4a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bggn4a16.png" );
    }

    // bgwn6a08 - 3x8 bits rgb color, alpha, white background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "bgwn6a08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgwn6a08.png" );
    }

    // bgyn6a16 - 3x16 bits rgb color, alpha, yellow background chunk
    {
        rgba16_image_t img;

        read_image( png_in + "bgyn6a16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "bgyn6a16.png" );
    }
}

BOOST_AUTO_TEST_CASE( transparency_test )
{
    image_read_settings< png_tag > settings;
    settings._read_transparency_data = true;

    // tbbn1g04 - transparent, black background chunk 
    {
        gray_alpha8_image_t img;

        // This file is missing
        //read_image( png_in + "tbbn1g04.png", img, tag_t() );
        //BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        //BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        //write( img, "tbbn1g04.png" );
    }

    // tbbn2c16 - transparent, blue background chunk 
    {
        rgba16_image_t img;

        read_image( png_in + "tbbn2c16.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbbn2c16.png" );
    }

    // tbbn3p08 - transparent, black background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "tbbn3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbbn3p08.png" );
    }

    // tbgn2c16 - transparent, green background chunk 
    {
        rgba16_image_t img;

        read_image( png_in + "tbgn2c16.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbgn2c16.png" );
    }

    // tbgn3p08 - transparent, light-gray background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "tbgn3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbgn3p08.png" );
    }

    // tbrn2c08 - transparent, red background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "tbrn2c08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbrn2c08.png" );
    }

    // tbwn1g16 - transparent, white background chunk 
    {
        gray_alpha16_image_t img;

        read_image( png_in + "tbwn1g16.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbwn1g16.png" );
    }

    // tbwn3p08 - transparent, white background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "tbwn3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbwn3p08.png" );
    }

    // tbyn3p08 - transparent, yellow background chunk 
    {
        rgba8_image_t img;

        read_image( png_in + "tbyn3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tbyn3p08.png" );
    }

    // tp0n1g08 - not transparent for reference (logo on gray) 
    {
        gray8_image_t img;

        read_image( png_in + "tp0n1g08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tp0n1g08.png" );
    }

    // tp0n2c08 - not transparent for reference (logo on gray) 
    {
        rgb8_image_t img;

        read_image( png_in + "tp0n2c08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tp0n2c08.png" );
    }

    // tp0n3p08 - not transparent for reference (logo on gray) 
    {
        rgb8_image_t img;

        read_image( png_in + "tp0n3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tp0n3p08.png" );
    }

    // tp1n3p08 - transparent, but no background chunk
    {
        rgba8_image_t img;

        read_image( png_in + "tp1n3p08.png", img, settings );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "tp1n3p08.png" );
    }
}

BOOST_AUTO_TEST_CASE( gamma_test )
{
    // g03n0g16 - grayscale, file-gamma = 0.35 
    {
        gray16_image_t img;

        read_image( png_in + "g03n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g03n0g16.png" );
    }

    // g03n2c08 - color, file-gamma = 0.35 
    {
        rgb8_image_t img;

        read_image( png_in + "g03n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g03n2c08.png" );
    }

    // g03n3p04 - paletted, file-gamma = 0.35 
    {
        rgb8_image_t img;

        read_image( png_in + "g03n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g03n3p04.png" );
    }

    // g04n0g16 - grayscale, file-gamma = 0.45 
    {
        gray16_image_t img;

        read_image( png_in + "g04n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g04n0g16.png" );
    }

    // g04n2c08 - color, file-gamma = 0.45 
    {
        rgb8_image_t img;

        read_image( png_in + "g04n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g04n2c08.png" );
    }

    // g04n3p04 - paletted, file-gamma = 0.45 
    {
        rgb8_image_t img;

        read_image( png_in + "g04n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g04n3p04.png" );
    }

    // g05n0g16 - grayscale, file-gamma = 0.55 
    {
        gray16_image_t img;

        read_image( png_in + "g05n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g05n0g16.png" );
    }

    // g05n2c08 - color, file-gamma = 0.55 
    {
        rgb8_image_t img;

        read_image( png_in + "g05n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g05n2c08.png" );
    }

    // g05n3p04 - paletted, file-gamma = 0.55 
    {
        rgb8_image_t img;

        read_image( png_in + "g05n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g05n3p04.png" );
    }

    // g07n0g16 - grayscale, file-gamma = 0.70 
    {
        gray16_image_t img;

        read_image( png_in + "g07n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g07n0g16.png" );
    }

    // g07n2c08 - color, file-gamma = 0.70 
    {
        rgb8_image_t img;

        read_image( png_in + "g07n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g07n2c08.png" );
    }

    // g07n3p04 - paletted, file-gamma = 0.70 
    {
        rgb8_image_t img;

        read_image( png_in + "g07n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g07n3p04.png" );
    }

    // g10n0g16 - grayscale, file-gamma = 1.00 
    {
        gray16_image_t img;

        read_image( png_in + "g10n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g10n0g16.png" );
    }

    // g10n2c08 - color, file-gamma = 1.00 
    {
        rgb8_image_t img;

        read_image( png_in + "g10n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g10n2c08.png" );
    }

    // g10n3p04 - paletted, file-gamma = 1.00 
    {
        rgb8_image_t img;

        read_image( png_in + "g10n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g10n3p04.png" );
    }

    // g25n0g16 - grayscale, file-gamma = 2.50 
    {
        gray16_image_t img;

        read_image( png_in + "g25n0g16.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g25n0g16.png" );
    }

    // g25n2c08 - color, file-gamma = 2.50 
    {
        rgb8_image_t img;

        read_image( png_in + "g25n2c08.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g25n2c08.png" );
    }

    // g25n3p04 - paletted, file-gamma = 2.50
    {
        rgb8_image_t img;

        read_image( png_in + "g25n3p04.png", img, tag_t() );
        BOOST_CHECK_EQUAL( view( img ).width() , 32 );
        BOOST_CHECK_EQUAL( view( img ).height(), 32 );

        write( img, "g25n3p04.png" );
    }
}

BOOST_AUTO_TEST_SUITE_END()
