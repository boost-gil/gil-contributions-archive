#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
#define BOOST_GIL_IO_ENABLE_GRAY_ALPHA

#include <boost/cstdint.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/extension/io_new/png_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

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
    write_view( bmp_out + file_name
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


        BOOST_CHECK_EQUAL( info._gamma, 0    );
        BOOST_CHECK_EQUAL( info._x_res, 0    );
        BOOST_CHECK_EQUAL( info._y_res, 0    );

        BOOST_CHECK_EQUAL( info._sbits, png_color_8p( NULL ) );
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

BOOST_AUTO_TEST_SUITE_END()
