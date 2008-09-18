#include "stdafx.h"

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>

using namespace std;
using namespace boost::gil;

typedef jpeg_tag tag_t;

namespace jpeg_test {

BOOST_AUTO_TEST_CASE( jpeg_file_format_test )
{
    std::string filename( "..\\test_images\\jpg\\found online\\test.jpg" );

    {
        typedef rgb8_image_t image_t;
        image_t img;

        read_image( filename
                  , img
                  , tag_t()
                  );

        write_view( "..\\test\\tiff\\test4.tif"
                  , view( img )
                  , tag_t()
                  );
    }
}

} // namespace jpeg_test