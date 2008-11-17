// tiff_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>
#include <boost/type_traits/is_same.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost::gil;

typedef tiff_tag tag_t;

namespace tiff_test {

BOOST_AUTO_TEST_CASE( quicktest )
{
    std::string filename( "..\\test_images\\tiff\\libtiffpic\\depth\\flower-minisblack-06.tif" );

    {
        typedef bit_aligned_image1_type< 6, gray_layout_t >::type image_t;
        typedef image_t::view_t view_t;
        typedef view_t::x_iterator x_iterator;

        image_t img;

        read_image( filename
                  , img
                  , tag_t()
                  );

        typedef bit_aligned_image1_type< 1, gray_layout_t >::type dst_image_t;
        typedef dst_image_t::view_t dst_view_t;
        typedef dst_view_t::reference dst_ref_t;

        dst_image_t dst_img( view( img ).dimensions() );
        copy_and_convert_pixels( view( img ), view( dst_img ));

        write_view( "single_test_2.tiff"
                  , view( dst_img )
                  , tiff_tag()
                  );
    }

    {
        typedef bit_aligned_image1_type< 6, gray_layout_t >::type image_t;
        typedef image_t::view_t view_t;
        typedef view_t::x_iterator x_iterator;

        image_t img( 10, 10 );
        fill_pixels( view( img ), 0 );

        for( int y = 0; y < 10; ++y )
        {
            x_iterator it = view( img ).row_begin( y );

            for( int x = 0; x < 10; ++x )
            {
                if( x == 0 || x == 1 || x == 4 || x == 5 || x == 8 || x == 9 )
                {
                    it[x] = 0;
                }
                else
                {
                    it[x] = 63;
                }
            }
        }

        typedef bit_aligned_image1_type< 1, gray_layout_t >::type dst_image_t;
        typedef dst_image_t::view_t dst_view_t;
        typedef dst_view_t::reference dst_ref_t;

        dst_image_t dst_img( view( img ).dimensions() );
        copy_and_convert_pixels( view( img ), view( dst_img ));

        write_view( "test_2.tiff"
                  , view( dst_img )
                  , tiff_tag()
                  );
    }
}

} // namespace tiff_test
