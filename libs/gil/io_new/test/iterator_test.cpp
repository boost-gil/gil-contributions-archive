
//#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
//#define BOOST_FILESYSTEM_VERSION 3
//#include <boost/filesystem/convenience.hpp>

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
    typedef get_reader< char*
                        , bmp_tag
                        , read_and_no_convert
                        , Scanline::view_t
                        >::type reader_t;

    reader_t bmp_reader = make_reader< char*
                                        , bmp_tag
                                        , read_and_no_convert
                                        , Scanline::view_t
                                        >
                                        ( in
                                        , bmp_tag()
                                        , read_and_no_convert()
                                        );

    //fill_pixels(view(scanline), Scanline::value_type());

    image_read_iterator< reader_t
                        , Scanline::view_t
                        > it( bmp_reader );

                
    Scanline scanline( 128, 1 );

    it.set_view( view( scanline ));

    reader_t::backend_t backend = it.backend();
    Image dst( backend._info._width, backend._info._height );

    for( int i = 0; i < static_cast<int>(view(dst).height()); ++i )
    {
        copy_pixels( subimage_view(       *it, 0, 0, static_cast<int>(view(dst).width()), 1 )
                    , subimage_view( view(dst), 0, i, static_cast<int>(view(dst).width()), 1 )
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
    //bmp_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\bmp\\g08os2.bmp", "c:\\chhenning\\8_5.bmp");
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

template< typename Scanline
        , typename Image
        >
void png_test( char* in
             , const char* out
             )
{
    typedef get_reader< char*
                        , png_tag
                        , read_and_no_convert
                        , Scanline::view_t
                        >::type reader_t;

    reader_t png_reader = make_reader< char*
                                     , png_tag
                                     , read_and_no_convert
                                     , Scanline::view_t
                                     >
                                     ( in
                                     , png_tag()
                                     , read_and_no_convert()
                                     );

    image_read_iterator< reader_t
                        , Scanline::view_t
                        > it( png_reader );

    int scanline_size = it.scanline_length();
}


BOOST_AUTO_TEST_CASE( png_test_cases )
{
    //png_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\png\\pngsuite\\PNGSUITE.png", "c:\\chhenning\\1.png");
    png_test< rgb8_image_t, rgb8_image_t  >("C:\\gil_contributions\\test_images\\png\\world_topo\\world.topo.bathy.200401.3x21600x21600.D1.png", "c:\\chhenning\\1.png");
}

BOOST_AUTO_TEST_SUITE_END()