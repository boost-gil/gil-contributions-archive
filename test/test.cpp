// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gil/extension/io/bmp_io.hpp>

using namespace gil;

int _tmain(int argc, _TCHAR* argv[])
{
///////////////////
// 1-bit images
///////////////////
   {
      // black and white palette ( #000000, #FFFFFF )

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g01bw.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      gray8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g01bw.bmp", view( image ));

      bmp_write_view( "..\\g01bw.bmp", view( image ));
   }

   {
      // white and black palette (#FFFFFF,#000000). Should look the same as g01bw, not inverted.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g01wb.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      gray8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g01wb.bmp", view( image ));

      bmp_write_view( "..\\g01wb.bmp", view( image ));
   }

   {
      // blue and green palette (#4040FF,#40FF40)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g01bg.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g01bg.bmp", view( image ));

      bmp_write_view( "..\\g01bg.bmp", view( image ));
   }

   {
      // blue and green palette (#4040FF,#40FF40)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g01p1.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g01p1.bmp", view( image ));

      bmp_write_view( "..\\g01p1.bmp", view( image ));
   }

///////////////////
// 4-bit images
///////////////////

   {
      // basic 4bpp (16 color) image

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g04.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g04.bmp", view( image ));

      bmp_write_view( "..\\g04.bmp", view( image ));
   }

   {
      // RLE compressed

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g04rle.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );

      try
      {
         bmp_read_view( "..\\test images\\g04rle.bmp", view( image ));

         bmp_write_view( "..\\g04rle.bmp", view( image ));
      }
      catch( std::ios_base::failure )
      {
         // not supported yet.
         assert( true );
      }
   }

   {
      // 4-color grayscale palette

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g04p4.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      gray8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g04p4.bmp", view( image ));

      bmp_write_view( "..\\g04p4.bmp", view( image ));
   }

///////////////////
// 8-bit images
///////////////////
   {
      // basic 8bpp (256 color) image

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08.bmp", view( image ));

      bmp_write_view( "..\\g08.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=0 [=256]

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08p256.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08p256.bmp", view( image ));

      bmp_write_view( "..\\g08p256.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=256

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08pi256.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08pi256.bmp", view( image ));

      bmp_write_view( "..\\g08pi256.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=64. It's barely possible that some 
      // sophisticated viewers may display this image in grayscale,
      // if there are a limited number of colors available.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08pi64.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08pi64.bmp", view( image ));

      bmp_write_view( "..\\g08pi64.bmp", view( image ));
   }

   {
      // RLE compressed.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08rle.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );

      try
      {
         bmp_read_view( "..\\test images\\g08rle.bmp", view( image ));

         bmp_write_view( "..\\g08rle.bmp", view( image ));
      }
      catch( std::ios_base::failure )
      {
         assert( true );
      }
   }

   {
      // OS/2-style bitmap. This is an obsolete variety of BMP that is still 
      // encountered sometimes. It has 3-byte palette entries (instead of 4), 
      // and 16-bit width/height fields (instead of 32).

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08os2.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08os2.bmp", view( image ));

      bmp_write_view( "..\\g08os2.bmp", view( image ));
   }

   {
      // resolution 7874x7874 pixels/meter (200x200 dpi)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08res22.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08res22.bmp", view( image ));

      bmp_write_view( "..\\g08res22.bmp", view( image ));
   }

   {
      // resolution 3937x3937 pixels/meter (100x100 dpi)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08res11.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08res11.bmp", view( image ));

      bmp_write_view( "..\\g08res11.bmp", view( image ));
   }

   {
      // resolution 7874x3937 pixels/meter (200x100 dpi). 
      // Some programs (e.g. Imaging for Windows) may display this image 
      // stretched vertically, which is the optimal thing to do if the 
      // program is primarily a viewer, rather than an editor.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08res21.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08res21.bmp", view( image ));

      bmp_write_view( "..\\g08res21.bmp", view( image ));
   }

   {
      // bits size not given (set to 0). This is legal for uncompressed bitmaps.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08s0.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08s0.bmp", view( image ));

      bmp_write_view( "..\\g08s0.bmp", view( image ));
   }

   {
      // bfOffBits in header not set to the usual value. There are 100 extra unused 
      // bytes between palette and bits.

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08offs.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08offs.bmp", view( image ));

      bmp_write_view( "..\\g08offs.bmp", view( image ));
   }

   {
      // size 126x63 (right and bottom slightly clipped) 

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08w126.bmp" );
      assert( dim.x == 126 && dim.y == 63 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08w126.bmp", view( image ));

      bmp_write_view( "..\\g08w126.bmp", view( image ));
   }

   {
      // size 125x62

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08w125.bmp" );
      assert( dim.x == 125 && dim.y == 62 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08w125.bmp", view( image ));

      bmp_write_view( "..\\g08w125.bmp", view( image ));
   }

   {
      // size 124x61

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08w124.bmp" );
      assert( dim.x == 124 && dim.y == 61 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08w124.bmp", view( image ));

      bmp_write_view( "..\\g08w124.bmp", view( image ));
   }

   {
      // 64-color grayscale palette

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g08p64.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      gray8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g08p64.bmp", view( image ));

      bmp_write_view( "..\\g08p64.bmp", view( image ));
   }

///////////////////
// 16-bit images
///////////////////
   {
      // 15-bit color (1 bit wasted), biCompression=BI_RGB (no bitfields, defaults to 5-5-5)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g16def555.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g16def555.bmp", view( image ));

      bmp_write_view( "..\\g16def555.bmp", view( image ));
   }

   {
      // 15-bit color, biCompression=BI_BITFIELDS (bitfields indicate 5-5-5)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g16bf555.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g16bf555.bmp", view( image ));

      bmp_write_view( "..\\g16bf555.bmp", view( image ));
   }

   {
      // 16-bit color, biCompression=BI_BITFIELDS (bitfields indicate 5-6-5)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g16bf565.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g16bf565.bmp", view( image ));

      bmp_write_view( "..\\g16bf565.bmp", view( image ));
   }

///////////////////
// 24-bit images
///////////////////
   {
      // 24-bit color (BGR)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g24.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgb8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g24.bmp", view( image ));

      bmp_write_view( "..\\g24.bmp", view( image ));
   }

///////////////////
// 32-bit images
///////////////////
   {
      // 24-bit color (8 bits wasted), biCompression=BI_RGB (no bitfields, defaults to BGRx)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g32def.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgba8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g32def.bmp", view( image ));

      bmp_write_view( "..\\g32def.bmp", view( image ));
   }

   {
      // 24-bit color (8 bits wasted), biCompression=BI_BITFIELDS (bitfields indicate BGRx)

      point2<int> dim = bmp_read_dimensions( "..\\test images\\g32bf.bmp" );
      assert( dim.x == 127 && dim.y == 64 );

      rgba8_image_t image( dim.x, dim.y );
      bmp_read_view( "..\\test images\\g32bf.bmp", view( image ));

      bmp_write_view( "..\\g32bf.bmp", view( image ));
   }

	return 0;
}

