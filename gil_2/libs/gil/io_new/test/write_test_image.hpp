#ifndef BOOST_GIL_IO_UNIT_TEST_WRITE_TEST_IMAGE_HPP
#define BOOST_GIL_IO_UNIT_TEST_WRITE_TEST_IMAGE_HPP

template< typename String
        , typename View
        , typename Tag
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void write_test_view( const String& file_name
                    , const View&   view
                    , const Tag&
                    )
#else
void write_test_view( const String&
                    , const View&
                    , const Tag&
                    )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    write_view( file_name, view, Tag() );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void bmp_write_test_view( const String& file_name
                        , const View&   view
                        )
#else
void bmp_write_test_view( const String&
                        , const View&
                        )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    bmp_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void jpeg_write_test_view( const String& file_name
                         , const View&   view
                         )
#else
void jpeg_write_test_view( const String&
                         , const View&
                         )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    jpeg_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void png_write_test_view( const String& file_name
                        , const View&   view
                        )
#else
void png_write_test_view( const String&
                        , const View&
                        )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    png_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void pnm_write_test_view( const String& file_name
                        , const View&   view
                        )
#else
void pnm_write_test_view( const String&
                        , const View&
                        )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    pnm_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void targa_write_test_view( const String& file_name
                          , const View&   view
                          )
#else
void targa_write_test_view( const String&
                          , const View&
                          )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    targa_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

template< typename String
        , typename View
        >
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
void tiff_write_test_view( const String& file_name
                         , const View&   view
                         )
#else
void tiff_write_test_view( const String&
                         , const View&
                         )
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
{
#ifdef BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
    tiff_write_view( file_name, view );
#endif // BOOST_GIL_IO_TEST_ALLOW_WRITING_IMAGES
}

#endif // BOOST_GIL_IO_UNIT_TEST_WRITE_TEST_IMAGE_HPP
