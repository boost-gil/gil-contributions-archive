
The problem with read_and_convert_view is probably not because of an interface error, but
rather the fact that the compiler screws up. The following test app serves as a proof:

Try compiling the following code.

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_read.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main(int argc, char *argv[])
{
    string filename( ".\\test_images\\tiff\\test.tif" );

    rgb8_image_t img( 200, 133 );
    read_and_convert_view( filename, view( img ), tiff_tag() );

   return 0;
}

The compiler should generate errors using MSVC 8. 

1>c:\gil\gil_2\boost\gil\extension\io_new\detail\tiff_io_read.hpp(103) : error C2039: 'get_property' : is not a member of 'std::basic_string<_Elem,_Traits,_Ax>'
[snip]

The reason is that the compiler is using the wrong io function which should be guarded by an enable_if
statement. But it's not working here.

To make the above code compile, one has to comment out some of the header files in 
tiff_read.hpp and viola it works!

/*
#include "detail/read_image_info.hpp"
#include "detail/read_view.hpp"
#include "detail/read_image.hpp"
#include "detail/read_and_convert_image.hpp"
*/
#include "detail/read_and_convert_view.hpp"

Note the only header that's not commented out is the one we need right now.

