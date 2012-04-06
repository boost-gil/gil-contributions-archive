
//#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
//#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>

#include <boost/gil/extension/io_new/detail/image_read_iterator.hpp>

#include <boost/gil/extension/io_new/detail/get_read_device.hpp>
#include <boost/gil/extension/io_new/detail/get_write_device.hpp>

#include <boost/gil/extension/io_new/detail/make_reader.hpp>
#include <boost/gil/extension/io_new/detail/make_writer.hpp>

#include <boost/test/unit_test.hpp>

namespace boost { namespace gil {

template< typename T
        , typename FormatTag
        , typename ConversionPolicy
        >
void test_get_reader( T&                      file
                    , const FormatTag&        format_tag
                    , const ConversionPolicy& conversion_policy
                    )
{
    get_reader< T
              , FormatTag
              >::type reader = make_reader( file
                                          , format_tag
                                          , conversion_policy
                                          );
}

template< typename T
        , typename FormatTag
        >
void test_get_writer( T&               file
                    , const FormatTag& format_tag
                    )
{
    get_writer< T
              , FormatTag
              >::type writer = make_writer( file
                                          , format_tag
                                          );
}


} // namespace gil
} // namespace boost

using namespace std;
using namespace boost;
using namespace gil;
namespace fs = boost::filesystem;

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

BOOST_AUTO_TEST_SUITE( iterator_test )

BOOST_AUTO_TEST_CASE( iterator_test_cases )
{
    typedef get_reader< char*
                      , bmp_tag
                      , read_and_no_convert
                      , bgr8_image_t::view_t
                      >::type reader_t;

    reader_t bmp_reader = make_reader< char*
                                     , bmp_tag
                                     , read_and_no_convert
                                     , bgr8_image_t::view_t
                                     >
                                     ( "C:\\gil_contributions\\test_images\\bmp\\rgb.bmp"
                                     , bmp_tag()
                                     , read_and_no_convert()
                                     );

    bgr8_image_t scanline( 128, 1 );
    fill_pixels(view(scanline), bgr8_pixel_t(0,0,0));

    rgb8_image_t dst( 127, 64 );

    image_read_iterator< reader_t
                       , bgr8_image_t::view_t
                       > it( bmp_reader, view( scanline ));


    for( int i = 0; i < 64; ++i )
    {
        copy_pixels( subimage_view(       *it, 0, 0, static_cast<int>(view(dst).width()), 1 )
                   , subimage_view( view(dst), 0, i, static_cast<int>(view(dst).width()), 1 )
                   );
    }

    write_view( "c:\\users\\chenning\\out.bmp", view(dst), bmp_tag() );



    //test_get_read_device< char*, bmp_tag, gil::detail::file_stream_device< bmp_tag > >();

    //test_get_read_device< char*, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_read_device< string, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_read_device< wstring, png_tag, gil::detail::file_stream_device< png_tag > >();

    //test_get_read_device< FILE*, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_read_device< ifstream, png_tag, gil::detail::istream_device< png_tag  > >();
    //test_get_read_device< TIFF*, tiff_tag, gil::detail::file_stream_device< tiff_tag > >();
    //test_get_read_device< fs::path, png_tag, gil::detail::file_stream_device< png_tag > >();


    //test_get_write_device< char*, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_write_device< string, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_write_device< wstring, png_tag, gil::detail::file_stream_device< png_tag > >();

    //test_get_write_device< FILE*, png_tag, gil::detail::file_stream_device< png_tag > >();
    //test_get_write_device< ofstream, png_tag, gil::detail::ostream_device< png_tag  > >();
    //test_get_write_device< TIFF*, tiff_tag, gil::detail::file_stream_device< tiff_tag > >();
    //test_get_write_device< fs::path, png_tag, gil::detail::file_stream_device< png_tag > >();

    //test_get_reader( "C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png"
    //               , png_tag()
    //               , read_and_no_convert()
    //               );

    //test_get_reader( string( "C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png" )
    //               , png_tag()
    //               , read_and_no_convert()
    //               );

    //test_get_reader( L"C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png"
    //               , png_tag()
    //               , read_and_no_convert()
    //               );

    //test_get_reader( wstring( L"C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png" )
    //               , png_tag()
    //               , read_and_no_convert()
    //               );

    //test_get_reader( fs::path( "C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png" )
    //               , png_tag()
    //               , read_and_no_convert()
    //               );

    //test_get_reader( std::ifstream("C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png", std::ios::binary )
    //                , png_tag()
    //                , read_and_no_convert()
    //                );

    //FILE* file = fopen( "C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png", "rb" );
    //test_get_reader( file
    //                , png_tag()
    //                , read_and_no_convert()
    //                );

    //test_get_writer( "C:\\gil_contributions\\output\\png\\test_get_writer.png"
    //               , png_tag()
    //               );

    //test_get_writer( string( "C:\\gil_contributions\\output\\png\\test_get_writer.png" )
    //               , png_tag()
    //               );

    //test_get_writer( wstring( L"C:\\gil_contributions\\output\\png\\test_get_writer.png" )
    //               , png_tag()
    //               );

    //test_get_writer( fs::path( "C:\\gil_contributions\\output\\png\\test_get_writer.png" )
    //               , png_tag()
    //               );

    //test_get_writer( std::ofstream( "C:\\gil_contributions\\output\\png\\test_get_writer.png", std::ios::binary )
    //               , png_tag()
    //               );

    //file = fopen( "C:\\gil_contributions\\output\\png\\test_get_writer.png", "wb" );
    //test_get_writer( file
    //               , png_tag()
    //               );


    //// png reader
    //get_reader< char*
    //          , png_tag
    //          >::type png_reader = make_reader( "C:\\gil_contributions\\test_images\\png\\wikipedia\\test.png"
    //                                            , png_tag()
    //                                            , read_and_no_convert()
    //                                            );

    //boost::gil::reader_backend< get_read_device< char*
    //                                           , png_tag
    //                                           >::type
    //                          , png_tag
    //                          > png_reader_backend = png_reader;

    //// bmp reader
    //get_reader< char*
    //          , bmp_tag
    //          >::type bmp_reader = make_reader( "C:\\gil_contributions\\test_images\\bmp\\rgb.bmp"
    //                                            , bmp_tag()
    //                                            , read_and_no_convert()
    //                                            );

    //boost::gil::reader_backend< get_read_device< char*
    //                                           , bmp_tag
    //                                           >::type
    //                          , bmp_tag
    //                          > bmp_reader_backend = bmp_reader;

    //// bmp writer
    //get_writer< char*
    //          , bmp_tag
    //          >::type bmp_writer = make_writer( "C:\\gil_contributions\\output\\bmp\\test_get_writer.bmp"
    //                                            , bmp_tag()
    //                                            );

    //boost::gil::writer_backend< get_write_device< char*
    //                                            , bmp_tag
    //                                            >::type
    //                           , bmp_tag
    //                           > bmp_writer_backend = bmp_writer;

    //// png writer
    //get_writer< char*
    //          , png_tag
    //          >::type png_writer = make_writer( "C:\\gil_contributions\\output\\png\\test_get_writer.png"
    //                                            , png_tag()
    //                                            );

    //boost::gil::writer_backend< get_write_device< char*
    //                                            , png_tag
    //                                            >::type
    //                           , png_tag
    //                           > png_writer_backend = png_writer;
}

BOOST_AUTO_TEST_SUITE_END()