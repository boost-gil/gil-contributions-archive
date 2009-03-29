#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_write.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/type_traits/is_same.hpp>

using namespace std;
using namespace boost::gil;

typedef bmp_tag tag_t;

namespace bmp_test {

const std::string filename( "..\\test_images\\bmp\\rgb.bmp" );

BOOST_AUTO_TEST_CASE( read_test )
{
}

} // namespace bmp_test
