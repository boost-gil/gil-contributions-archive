/**************************************************
 *
 * This test file will read and write various
 * tiff formats. 
 *
 **************************************************/

#include <boost/gil/extension/io_new/tiff_all.hpp>

#include <boost/test/unit_test.hpp>

#include "paths.hpp"

using namespace std;
using namespace boost::gil;

typedef tiff_tag tag_t;

namespace tiff_test {

// 64K-colormap.tiff
BOOST_AUTO_TEST_CASE( colormap_test )
{
    std::string filename( tiff_in + "GraphicsMagick\\64K-colormap.tiff" );

    {
        image_read_info< tag_t > info = read_image_info( filename, tag_t() );

        typedef rgb16_image_t image_t;
        image_t img;

        read_image( filename
                  , img
                  , tag_t()
                  );

        write_view( tiff_out + "test40.tif"
                  , view( img )
                  , tiff_tag()
                  );
    }
}

} // namespace tiff_test
