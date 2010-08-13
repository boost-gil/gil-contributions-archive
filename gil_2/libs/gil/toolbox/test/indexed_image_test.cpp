// indexed_image_test.cpp
//

#include "stdafx.h"

#include <boost/gil/extension/toolbox/hsl.hpp>
#include <boost/gil/extension/toolbox/hsv.hpp>

using namespace std;
using namespace boost;
using namespace gil;
using namespace toolbox;



int _tmain(int argc, _TCHAR* argv[])
{
    {
        indexed_image< uint8_t, rgb8_pixel_t > img( 640, 480 );
        fill_pixels( view( img ), rgb8_pixel_t( 255, 0, 0 ));

        rgb8_pixel_t p = *view( img ).xy_at( 10, 10 );
    }

    {

        indexed_image< gray8_pixel_t, rgb8_pixel_t > img( 640, 480, 256 );

        generate_pixels( img.get_indices_view()
                       , [] () -> uint8_t
                       {
                            static int i = -1;
                            i = ( i == 256 ) ? 0 : ++i;

                            return gray8_pixel_t( i );
                       }
                       );


        generate_pixels( img.get_palette_view()
                       , [] () ->rgb8_pixel_t
                       {
                          static int i = -1;
                          i = ( i == 256 ) ? 0 : ++i;

                          return rgb8_pixel_t( i, i, i );
                       }
                       );

        int i = ( 640 * 10 + 10 ) % 256;

        gray8_pixel_t index = *img.get_indices_view().xy_at( 10   , 1 );
        rgb8_pixel_t  color = *img.get_palette_view().xy_at( index, 0 );

        rgb8_pixel_t p = *view( img ).xy_at( 10, 1 );

        i = 9;
    }

    {
        indexed_image< uint8_t, rgb8_pixel_t > img( 640, 480, 256 );

        generate_pixels( img.get_indices_view()
                       , [] () -> uint8_t
                       {
                            static int i = -1;
                            i = ( i == 256 ) ? 0 : ++i;

                            return i;
                       }
                       );


        generate_pixels( img.get_palette_view()
                       , [] () ->rgb8_pixel_t
                       {
                          static int i = -1;
                          i = ( i == 256 ) ? 0 : ++i;

                          return rgb8_pixel_t( i, i, i );
                       }
                       );

        int i = ( 640 * 10 + 10 ) % 256;

        uint8_t      index = *img.get_indices_view().xy_at( 10   , 1 );
        rgb8_pixel_t color = *img.get_palette_view().xy_at( index, 0 );

        rgb8_pixel_t p = *view( img ).xy_at( 10, 1 );

        i = 9;
    }

    {
        typedef indexed_image< uint8_t, rgb8_pixel_t > image_t;
        image_t img( 640, 480, 256 );

        for( int y=0; y < view( img ).height(); ++y )
        {
            image_t::view_t::x_iterator it = view( img ).row_begin( y );

            for( int x = 0; x < view( img ).width(); ++x )
            {
                rgb8_pixel_t p = *it;
                it++;
            }
        }
    }

	return 0;
}

