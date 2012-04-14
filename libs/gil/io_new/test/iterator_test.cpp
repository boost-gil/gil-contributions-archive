
//#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
//#define BOOST_FILESYSTEM_VERSION 3
//#include <boost/filesystem/convenience.hpp>

#define BOOST_GIL_IO_ENABLE_GRAY_ALPHA

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>

#include <boost/gil/extension/io_new/detail/image_read_iterator.hpp>

#include <boost/gil/extension/io_new/detail/get_read_device.hpp>
#include <boost/gil/extension/io_new/detail/get_write_device.hpp>

#include <boost/gil/extension/io_new/detail/make_reader.hpp>
#include <boost/gil/extension/io_new/detail/make_writer.hpp>

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace gil;
//namespace fs = boost::filesystem;

BOOST_AUTO_TEST_SUITE( bmp_test )

template< typename Scanline
        , typename Image
        >
void bmp_test( char* in
             , const char* out
             )
{
    typedef get_scanline_reader< char*
                               , bmp_tag
                               >::type reader_t;

    reader_t bmp_reader = make_scanline_reader< char*
                                              , bmp_tag
                                              >
                                              ( in
                                              , bmp_tag()
                                              );

    scanline_read_iterator< reader_t > it( bmp_reader );

                
    Scanline scanline( 128, 1 );

    it.set_buffer( &view( scanline )[0][0]);

    reader_t::backend_t backend = it.backend();
    Image dst( backend._info._width, backend._info._height );

    for( int i = 0; i < static_cast<int>(view(dst).height()); ++i )
    {
        *it;

        copy_pixels( subimage_view(  view( scanline ), 0, 0, static_cast<int>(view(dst).width()), 1 )
                    , subimage_view(      view( dst ), 0, i, static_cast<int>(view(dst).width()), 1 )
                    );
    }

    write_view( out, view(dst), bmp_tag() );
}

BOOST_AUTO_TEST_CASE( bmp_test_cases )
{
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g01bg.bmp", "c:\\chhenning\\1.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g01bw.bmp", "c:\\chhenning\\1_1.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g01p1.bmp", "c:\\chhenning\\1_2.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g01wb.bmp", "c:\\chhenning\\1_3.bmp");

    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g04.bmp", "c:\\chhenning\\4.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g04p4.bmp", "c:\\chhenning\\4_2.bmp");

    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08.bmp", "c:\\chhenning\\8.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08p256.bmp", "c:\\chhenning\\8_1.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08pi256.bmp", "c:\\chhenning\\8_2.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08pi64.bmp", "c:\\chhenning\\8_3.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08os2.bmp", "c:\\chhenning\\8_5.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08res22.bmp", "c:\\chhenning\\8_6.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08res11.bmp", "c:\\chhenning\\8_7.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08res21.bmp", "c:\\chhenning\\8_8.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08s0.bmp", "c:\\chhenning\\8_9.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08offs.bmp", "c:\\chhenning\\8_10.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08w126.bmp", "c:\\chhenning\\8_11.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08w125.bmp", "c:\\chhenning\\8_12.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08w124.bmp", "c:\\chhenning\\8_13.bmp");
    //bmp_test< rgba8_image_t, rgba8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08p64.bmp", "c:\\chhenning\\8_14.bmp");

    //bmp_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g16def555.bmp", "c:\\chhenning\\16.bmp");
    //bmp_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g16bf555.bmp", "c:\\chhenning\\16_1.bmp");
    //bmp_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g16bf565.bmp", "c:\\chhenning\\16_2.bmp");
    //
    //bmp_test<  bgr8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g24.bmp", "c:\\chhenning\\24.bmp");
    //bmp_test< bgra8_image_t, rgba8_image_t >("C:\\gil_contributions\\test_images\\bmp\\g32bf.bmp", "c:\\chhenning\\32.bmp");
    //bmp_test< bgra8_image_t, rgba8_image_t >("C:\\gil_contributions\\test_images\\bmp\\g32def.bmp", "c:\\chhenning\\32_1.bmp");
}

template< typename Image >
void png_test( char* in
             , const char* out
             )
{
    typedef get_scanline_reader< char*
                               , png_tag
                               >::type reader_t;

    reader_t png_reader = make_scanline_reader< char*
                                              , png_tag
                                              >
                                              ( in
                                              , png_tag()
                                              );

    scanline_read_iterator< reader_t > it( png_reader );
    reader_t::backend_t backend = it.backend();

    Image scanline(backend._info._width, 1 );

    Image::view_t::x_iterator buffer_it = view(scanline).row_begin(0);
    byte_t* data = (byte_t*) &gil::at_c<0>(*buffer_it);

    it.set_buffer( data );

    Image dst( backend._info._width, backend._info._height );

    for( unsigned int i = 0; i < backend._info._height; ++i )
    {
        *it;

        copy_pixels( subimage_view(  view( scanline ), 0, 0, static_cast<int>(view(dst).width()), 1 )
                    , subimage_view(      view( dst ), 0, i, static_cast<int>(view(dst).width()), 1 )
                    );
    }

    write_view( out, view(dst), png_tag() );

}


BOOST_AUTO_TEST_CASE( png_test_cases )
{
    png_test< gray1_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN0G01.PNG", "c:\\chhenning\\1.png");
    png_test< gray2_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN0G02.PNG", "c:\\chhenning\\2.png");
    png_test< gray4_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN0G04.PNG", "c:\\chhenning\\3.png");
    png_test< gray8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN0G08.PNG", "c:\\chhenning\\4.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN0G16.PNG", "c:\\chhenning\\5.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN2C08.PNG", "c:\\chhenning\\6.png");
    png_test< rgb16_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN2C16.PNG", "c:\\chhenning\\7.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN3P01.PNG", "c:\\chhenning\\8.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN3P02.PNG", "c:\\chhenning\\9.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN3P04.PNG", "c:\\chhenning\\10.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN3P08.PNG", "c:\\chhenning\\11.png");
    png_test< gray_alpha8_image_t > ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN4A08.PNG", "c:\\chhenning\\12.png");
    png_test< gray_alpha16_image_t >( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN4A16.PNG", "c:\\chhenning\\13.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN6A08.PNG", "c:\\chhenning\\14.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASN6A16.PNG", "c:\\chhenning\\15.png");
    png_test< gray1_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI0G01.PNG", "c:\\chhenning\\16.png");
    png_test< gray2_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI0G02.PNG", "c:\\chhenning\\17.png");
    png_test< gray4_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI0G04.PNG", "c:\\chhenning\\18.png");
    png_test< gray8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI0G08.PNG", "c:\\chhenning\\19.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI0G16.PNG", "c:\\chhenning\\21.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI2C08.PNG", "c:\\chhenning\\22.png");
    png_test< rgb16_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI2C16.PNG", "c:\\chhenning\\23.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI3P01.PNG", "c:\\chhenning\\24.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI3P02.PNG", "c:\\chhenning\\25.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI3P04.PNG", "c:\\chhenning\\26.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI3P08.PNG", "c:\\chhenning\\27.png");
    png_test< gray_alpha8_image_t > ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI4A08.PNG", "c:\\chhenning\\28.png");
    png_test< gray_alpha16_image_t >( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI4A16.PNG", "c:\\chhenning\\29.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI6A08.PNG", "c:\\chhenning\\31.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BASI6A16.PNG", "c:\\chhenning\\32.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S01I3P01.PNG", "c:\\chhenning\\33.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S01N3P01.PNG", "c:\\chhenning\\34.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S02I3P01.PNG", "c:\\chhenning\\35.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S02N3P01.PNG", "c:\\chhenning\\36.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S03I3P01.PNG", "c:\\chhenning\\37.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S03N3P01.PNG", "c:\\chhenning\\38.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S04I3P01.PNG", "c:\\chhenning\\39.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S04N3P01.PNG", "c:\\chhenning\\41.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S05I3P02.PNG", "c:\\chhenning\\42.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S05N3P02.PNG", "c:\\chhenning\\43.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S06I3P02.PNG", "c:\\chhenning\\44.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S06N3P02.PNG", "c:\\chhenning\\45.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S07I3P02.PNG", "c:\\chhenning\\46.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S07N3P02.PNG", "c:\\chhenning\\47.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S08I3P02.PNG", "c:\\chhenning\\48.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S08N3P02.PNG", "c:\\chhenning\\49.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S09I3P02.PNG", "c:\\chhenning\\51.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S09N3P02.PNG", "c:\\chhenning\\52.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S32I3P04.PNG", "c:\\chhenning\\53.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S32N3P04.PNG", "c:\\chhenning\\54.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S33I3P04.PNG", "c:\\chhenning\\55.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S33N3P04.PNG", "c:\\chhenning\\56.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S34I3P04.PNG", "c:\\chhenning\\57.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S34N3P04.PNG", "c:\\chhenning\\58.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S35I3P04.PNG", "c:\\chhenning\\59.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S35N3P04.PNG", "c:\\chhenning\\61.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S36I3P04.PNG", "c:\\chhenning\\62.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S36N3P04.PNG", "c:\\chhenning\\63.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S37I3P04.PNG", "c:\\chhenning\\64.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S37N3P04.PNG", "c:\\chhenning\\65.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S38I3P04.PNG", "c:\\chhenning\\66.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S38N3P04.PNG", "c:\\chhenning\\67.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S39I3P04.PNG", "c:\\chhenning\\68.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S39N3P04.PNG", "c:\\chhenning\\69.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S40I3P04.PNG", "c:\\chhenning\\71.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\S40N3P04.PNG", "c:\\chhenning\\72.png");
    png_test< gray_alpha8_image_t > ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGAI4A08.PNG", "c:\\chhenning\\73.png");
    png_test< gray_alpha16_image_t >( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGAI4A16.PNG", "c:\\chhenning\\74.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGAN6A08.PNG", "c:\\chhenning\\75.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGAN6A16.PNG", "c:\\chhenning\\76.png");
    png_test< gray_alpha8_image_t > ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGBN4A08.PNG", "c:\\chhenning\\77.png");
    png_test< gray_alpha16_image_t >( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGGN4A16.PNG", "c:\\chhenning\\78.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGWN6A08.PNG", "c:\\chhenning\\79.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\BGYN6A16.PNG", "c:\\chhenning\\81.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBBN2C16.PNG", "c:\\chhenning\\82.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBBN3P08.PNG", "c:\\chhenning\\83.png");
    png_test< rgba16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBGN2C16.PNG", "c:\\chhenning\\84.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBGN3P08.PNG", "c:\\chhenning\\85.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBRN2C08.PNG", "c:\\chhenning\\86.png");
    png_test< gray_alpha16_image_t >( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBWN1G16.PNG", "c:\\chhenning\\87.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBWN3P08.PNG", "c:\\chhenning\\88.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TBYN3P08.PNG", "c:\\chhenning\\89.png");
    png_test< gray8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TP0N1G08.PNG", "c:\\chhenning\\91.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TP0N2C08.PNG", "c:\\chhenning\\92.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TP0N3P08.PNG", "c:\\chhenning\\93.png");
    png_test< rgba8_image_t >       ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\TP1N3P08.PNG", "c:\\chhenning\\94.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G03N0G16.PNG", "c:\\chhenning\\95.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G03N2C08.PNG", "c:\\chhenning\\96.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G03N3P04.PNG", "c:\\chhenning\\97.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G04N0G16.PNG", "c:\\chhenning\\98.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G04N2C08.PNG", "c:\\chhenning\\99.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G04N3P04.PNG", "c:\\chhenning\\101.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G05N0G16.PNG", "c:\\chhenning\\102.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G05N2C08.PNG", "c:\\chhenning\\103.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G05N3P04.PNG", "c:\\chhenning\\104.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G07N0G16.PNG", "c:\\chhenning\\106.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G07N2C08.PNG", "c:\\chhenning\\107.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G07N3P04.PNG", "c:\\chhenning\\108.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G10N0G16.PNG", "c:\\chhenning\\110.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G10N2C08.PNG", "c:\\chhenning\\111.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G10N3P04.PNG", "c:\\chhenning\\112.png");
    png_test< gray16_image_t >      ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G25N0G16.PNG", "c:\\chhenning\\113.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G25N2C08.PNG", "c:\\chhenning\\114.png");
    png_test< rgb8_image_t >        ( "C:\\gil_contributions\\test_images\\png\\pngsuite\\G25N3P04.PNG", "c:\\chhenning\\115.png");
}

BOOST_AUTO_TEST_SUITE_END()