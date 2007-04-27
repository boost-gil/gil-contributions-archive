// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/gil/extension/io/bmp_io.hpp>
#include <boost/gil/extension/toolbox/rgb_algorithms.hpp>

using namespace std;
using namespace boost;
using namespace gil;
using namespace toolbox;


int main(int argc, char* argv[])
{
   {
      // test negate
      rgb8_image_t src;
      bmp_read_image( ".\\flower.bmp", src );

      rgb8_image_t dst( src.dimensions() );

      negative( view( src )
              , view( dst ));

      bmp_write_view( ".\\flower_neg.bmp", view( dst ));
   }

   {
      // test negate
      rgb8_image_t src;
      bmp_read_image( ".\\flower.bmp", src );

      rgb8_image_t dst( src.dimensions() );

      brightness( view( src )
                , view( dst )
                , 0.5         );

      bmp_write_view( ".\\flower_brightness.bmp", view( dst ));
   }

  	return 0;
}