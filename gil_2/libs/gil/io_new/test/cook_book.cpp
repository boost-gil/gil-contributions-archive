
#include <fstream>

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
    typedef bmp_tag tag_t;

    std::ifstream in( bmp_filename.c_str(), ios::binary );

    typedef get_reader< std::ifstream
                       , tag_t
                       , detail::read_and_no_convert
                       >::type backend_t;


    //typedef get_reader_backend< std::ifstream
    //                          , tag_t
    //                          >::type backend_t;

    //backend_t backend = make_reader( in
    //                               , tag_t()
    //                               );

    //backend_t backend = read_image_info( in
    //                                   , tag_t()
    //                                   );
}

BOOST_AUTO_TEST_SUITE_END()
