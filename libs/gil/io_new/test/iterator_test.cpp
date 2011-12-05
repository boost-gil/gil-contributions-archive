
#define BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem/convenience.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>

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

}

BOOST_AUTO_TEST_SUITE_END()