// image_test.cpp : Defines the entry point for the console application.
//

#include <boost/gil/gil_all.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/planar_pixel_reference.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/color_convert.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/filesystem.hpp>

#define GIL_NO_BMP  //(un)comment this line to allow/disallow the BMP support 
#define GIL_NO_TIFF  //(un)comment this line to allow/disallow the BMP support 
#define GIL_NO_JPEG  //(un)comment this line to allow/disallow the BMP support 
#define GIL_NO_PNG  //(un)comment this line to allow/disallow the BMP support 


#ifndef GIL_NO_TIFF
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>
#endif

#ifndef GIL_NO_JPEG
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#endif

#ifndef GIL_NO_PNG
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#endif 

#ifndef GIL_NO_BMP
#include <boost/gil/extension/io/bmp_dynamic_io.hpp>
#endif

#ifndef GIL_NO_PNM
#include <boost/gil/extension/io/pnm_dynamic_io.hpp>
#endif



using namespace boost::gil;
using namespace std;

//this was formerly in any_image_impl -JCF
typedef any_image<
    boost::mpl::vector< 
       rgb8_image_t,
       argb8_image_t,
       gray8_image_t
    >
> any_image_t;
   


void test_image_io() {
    const string in_dir="";  // directory of source images
    const std::string out_dir=in_dir+"image_io-out/";


    namespace fs = boost::filesystem;
    if (! fs::exists(fs::path(out_dir)))
        fs::create_directory(fs::path(out_dir));


// *********************************** 
// ************************ GRAY IMAGE
// *********************************** 
#ifndef GIL_NO_TIFF
    gray8_image_t imgGray;
// TIFF
    // load gray tiff into gray image
    tiff_read_image(in_dir+"gray.tif",imgGray);
    // save gray image to tiff
    tiff_write_view(out_dir+"grayFromGray.tif",view(imgGray));

    // load RGB tiff into gray image
    tiff_read_and_convert_image(in_dir+"RGB.tif",imgGray);

    // save gray image to tiff (again!)
    tiff_write_view(out_dir+"grayFromRGB.tif",view(imgGray));
#endif

#ifndef GIL_NO_JPEG
// JPEG
    // load gray jpeg into gray image
    jpeg_read_image(in_dir+"gray.jpg",imgGray);
    // save gray image to gray jpeg
    jpeg_write_view(out_dir+"grayFromGray.jpg",view(imgGray));
    
    // load RGB jpeg into gray image
    jpeg_read_and_convert_image(in_dir+"RGB.jpg",imgGray);
    // save gray image to RGB jpeg
    jpeg_write_view(out_dir+"grayFromRGB.jpg",color_converted_view<rgb8_pixel_t>(view(imgGray)));
#endif

#ifndef GIL_NO_PNG
// PNG
    // load gray png into gray image
    png_read_image(in_dir+"gray.png",imgGray);
    // save gray image to gray png
    png_write_view(out_dir+"grayFromGray.png",view(imgGray));
    
    // load RGB png into gray image
    png_read_and_convert_image(in_dir+"RGB.png",imgGray);
    // save gray image to RGB png
    png_write_view(out_dir+"grayFromRGB.png",color_converted_view<rgb8_pixel_t>(view(imgGray)));
#endif 

// *********************************** 
// ************************* RGB Planar
// *********************************** 

    rgb8_image_t imgRGB;

#ifndef GIL_NO_TIFF
// TIFF

    // load gray tiff into RGB image
    tiff_read_and_convert_image(in_dir+"gray.tif",imgRGB);
    // save RGB image to tiff
    tiff_write_view(out_dir+"RGBFromGray.tif",view(imgRGB));

    // load RGB tiff into RGB image
    tiff_read_image(in_dir+"RGB.tif",imgRGB);
    // save RGB image to tiff (again!)
    tiff_write_view(out_dir+"RGBFromRGB.tif",view(imgRGB));
#endif 

#ifndef GIL_NO_JPEG
// JPEG
    // load gray jpeg into RGB image
    jpeg_read_and_convert_image(in_dir+"gray.jpg",imgRGB);
    // save RGB image to gray jpeg
    jpeg_write_view(out_dir+"RGBFromGray.jpg",view(imgRGB));
    
    // load RGB jpeg into RGB image
    jpeg_read_image(in_dir+"RGB.jpg",imgRGB);
    // save RGB image to RGB jpeg
    jpeg_write_view(out_dir+"RGBFromRGB.jpg",view(imgRGB));
#endif

#ifndef GIL_NO_PNG
// PNG
    // load gray png into RGB image
    png_read_and_convert_image(in_dir+"gray.png",imgRGB);
    // save RGB image to gray png
    png_write_view(out_dir+"RGBFromGray.png",view(imgRGB));
    
    // load RGB png into RGB image
    png_read_image(in_dir+"RGB.png",imgRGB);
    // save RGB image to RGB png
    png_write_view(out_dir+"RGBFromRGB.png",view(imgRGB));
#endif

// *********************************** 
// ************************ GRAY32 Planar
// *********************************** 
    gray32f_image_t imgGray32;

#ifndef GIL_NO_TIFF
// TIFF
    // load gray tiff into gray image
    tiff_read_and_convert_image(in_dir+"gray.tif",imgGray32);
    // save gray image to tiff
    tiff_write_view(out_dir+"gray32FromGray.tif",view(imgGray32));
        
    // load RGB tiff into gray image
    tiff_read_and_convert_image(in_dir+"RGB.tif",imgGray32);

    // save gray image to tiff (again!)
    tiff_write_view(out_dir+"gray32FromRGB.tif",view(imgGray32));
#endif

#ifndef GIL_NO_JPEG
// JPEG
    tiff_read_and_convert_image(in_dir+"gray.tif",imgGray32);    // again TIF (jpeg load not supported)
    // save RGB image to gray jpeg
    tiff_write_view(out_dir+"gray32FromGray.jpg",view(imgGray32));
    
    // load RGB jpeg into RGB image
    tiff_read_and_convert_image(in_dir+"RGB.tif",imgGray32);    // again TIF (jpeg load not supported)
    // save RGB image to RGB jpeg
    tiff_write_view(out_dir+"gray32FromRGB.jpg",color_converted_view<rgb32f_pixel_t>(view(imgGray32)));
#endif

// *********************************** 
// ************************ NATIVE Planar
// *********************************** 
    any_image_t anyImg;

#ifndef GIL_NO_TIFF
// TIFF
    // load RGB tiff into any image
    tiff_read_image(in_dir+"RGB.tif",anyImg);

    // save any image to tiff
    tiff_write_view(out_dir+"RGBNative.tif",view(anyImg));

    // load gray tiff into any image
    tiff_read_image(in_dir+"gray.tif",anyImg);
    
    // save any image to tiff
    tiff_write_view(out_dir+"grayNative.tif",view(anyImg));
#endif

#ifndef GIL_NO_JPEG
// JPEG
    // load gray jpeg into any image
    jpeg_read_image(in_dir+"gray.jpg",anyImg);
    // save any image to jpeg
    jpeg_write_view(out_dir+"grayNative.jpg",view(anyImg));
    
    // load RGB jpeg into any image
    jpeg_read_image(in_dir+"RGB.jpg",anyImg);
    // save any image to jpeg
    jpeg_write_view(out_dir+"RGBNative.jpg",view(anyImg));
#endif

#ifndef GIL_NO_PNG
// PNG
    // load gray png into any image
    png_read_image(in_dir+"gray.png",anyImg);
    // save any image to png
    png_write_view(out_dir+"grayNative.png",view(anyImg));
    
    // load RGB png into any image
    png_read_image(in_dir+"RGB.png",anyImg);
    // save any image to png
    png_write_view(out_dir+"RGBNative.png",view(anyImg));
#endif

// *********************************** 
// ************************ BMP Test
// *********************************** 
#ifndef GIL_NO_BMP
///////////////////
// 1-bit images
///////////////////
   {
      // black and white palette ( #000000, #FFFFFF )

      gray8_image_t image;
      bmp_read_image( in_dir+"g01bw.bmp", image);

      bmp_write_view( out_dir+"g01bw.bmp", view( image ));
   }

   {
      // white and black palette (#FFFFFF,#000000). Should look the same as g01bw, not inverted.

      gray8_image_t image;
      bmp_read_image( in_dir+"g01wb.bmp", image);

      bmp_write_view( out_dir+"g01wb.bmp", view( image ));
   }

   {
      // blue and green palette (#4040FF,#40FF40)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g01bg.bmp", image);

      bmp_write_view( out_dir+"g01bg.bmp", view( image ));
   }

   {
      // blue and green palette (#4040FF,#40FF40)
      rgb8_image_t image;
      bmp_read_image( in_dir+"g01p1.bmp", image);

      bmp_write_view( out_dir+"g01p1.bmp", view( image ));
   }

///////////////////
// 4-bit images
///////////////////

   {
      // basic 4bpp (16 color) image

      rgb8_image_t image;
      bmp_read_image( in_dir+"g04.bmp", image);

      bmp_write_view( out_dir+"g04.bmp", view( image ));
   }

   {
      /* not supported, yet
      // RLE compressed

      rgb8_image_t image;
      bmp_read_image( in_dir+"g04rle.bmp", image);

      bmp_write_view( out_dir+"g04rle.bmp", view( image ));
      */
   }

   {
      // 4-color grayscale palette

      gray8_image_t image;
      bmp_read_image( in_dir+"g04p4.bmp", image);

      bmp_write_view( out_dir+"g04p4.bmp", view( image ));
   }

///////////////////
// 8-bit images
///////////////////
   {
      // basic 8bpp (256 color) image

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08.bmp", image);

      bmp_write_view( out_dir+"g08.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=0 [=256]

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08p256.bmp", image);

      bmp_write_view( out_dir+"g08p256.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=256

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08pi256.bmp", image);

      bmp_write_view( out_dir+"g08pi256.bmp", view( image ));
   }

   {
      // biClrUsed=256, biClrImportant=64. It's barely possible that some 
      // sophisticated viewers may display this image in grayscale,
      // if there are a limited number of colors available.

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08pi64.bmp", image);

      bmp_write_view( out_dir+"g08pi64.bmp", view( image ));
   }

   {
      /* not supported, yet
      // RLE compressed.

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08rle.bmp", image);

      bmp_write_view( out_dir+"g08rle.bmp", view( image ));
      */
   }

   {
      // OS/2-style bitmap. This is an obsolete variety of BMP that is still 
      // encountered sometimes. It has 3-byte palette entries (instead of 4), 
      // and 16-bit width/height fields (instead of 32).

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08os2.bmp", image);

      bmp_write_view( out_dir+"g08os2.bmp", view( image ));
   }

   {
      // resolution 7874x7874 pixels/meter (200x200 dpi)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08res22.bmp", image);

      bmp_write_view( out_dir+"g08res22.bmp", view( image ));
   }

   {
      // resolution 3937x3937 pixels/meter (100x100 dpi)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08res11.bmp", image);

      bmp_write_view( out_dir+"g08res11.bmp", view( image ));
   }

   {
      // resolution 7874x3937 pixels/meter (200x100 dpi). 
      // Some programs (e.g. Imaging for Windows) may display this image 
      // stretched vertically, which is the optimal thing to do if the 
      // program is primarily a viewer, rather than an editor.

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08res21.bmp", image);

      bmp_write_view( out_dir+"g08res21.bmp", view( image ));
   }

   {
      // bits size not given (set to 0). This is legal for uncompressed bitmaps.

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08s0.bmp", image);

      bmp_write_view( out_dir+"g08s0.bmp", view( image ));
   }

   {
      // bfOffBits in header not set to the usual value. There are 100 extra unused 
      // bytes between palette and bits.

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08offs.bmp", image);

      bmp_write_view( out_dir+"g08offs.bmp", view( image ));
   }

   {
      // size 126x63 (right and bottom slightly clipped) 

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08w126.bmp", image);

      bmp_write_view( out_dir+"g08w126.bmp", view( image ));
   }

   {
      // size 125x62

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08w125.bmp", image);

      bmp_write_view( out_dir+"g08w125.bmp", view( image ));
   }

   {
      // size 124x61

      rgb8_image_t image;
      bmp_read_image( in_dir+"g08w124.bmp", image);

      bmp_write_view( out_dir+"g08w124.bmp", view( image ));
   }

   {
      // 64-color grayscale palette
      gray8_image_t image;
      bmp_read_image( in_dir+"g08p64.bmp", image);

      bmp_write_view( out_dir+"g08p64.bmp", view( image ));
   }

///////////////////
// 16-bit images
///////////////////
   {
      // 15-bit color (1 bit wasted), biCompression=BI_RGB (no bitfields, defaults to 5-5-5)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g16def555.bmp", image);

      bmp_write_view( out_dir+"g16def555.bmp", view( image ));
   }

   {
      // 15-bit color, biCompression=BI_BITFIELDS (bitfields indicate 5-5-5)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g16bf555.bmp", image);

      bmp_write_view( out_dir+"g16bf555.bmp", view( image ));
   }

   {
      // 16-bit color, biCompression=BI_BITFIELDS (bitfields indicate 5-6-5)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g16bf565.bmp", image);

      bmp_write_view( out_dir+"g16bf565.bmp", view( image ));
   }

///////////////////
// 24-bit images
///////////////////
   {
      // 24-bit color (BGR)

      rgb8_image_t image;
      bmp_read_image( in_dir+"g24.bmp", image);

      bmp_write_view( out_dir+"g24.bmp", view( image ));
   }

///////////////////
// 32-bit images
///////////////////
   {
      // 24-bit color (8 bits wasted), biCompression=BI_RGB (no bitfields, defaults to BGRx)

      rgba8_image_t image;
      bmp_read_image( in_dir+"g32def.bmp", image );

      bmp_write_view( out_dir+"g32def.bmp", view( image ));
   }

   {
      // 24-bit color (8 bits wasted), biCompression=BI_BITFIELDS (bitfields indicate BGRx)

      rgba8_image_t image;
      bmp_read_image( in_dir+"g32bf.bmp", image );

      bmp_write_view( out_dir+"g32bf.bmp", view( image ));
   }
#endif //GIL_NO_BMP

// *********************************** 
// ************************ PNM Test
// *********************************** 

#ifndef GIL_NO_PNM
   {
      // a PBMA file
      gray8_image_t image;
      pnm_read_image( in_dir+"p1.pnm", image );

      pnm_write_view( out_dir+"p1.pnm", view( image ));
   }

   {
      // a PGMA file
      rgb8_image_t image;
      pnm_read_image( in_dir+"p2.pnm", image );

      pnm_write_view( out_dir+"p2.pnm", view( image ));
   }

   {
      // a PPMB file
      rgb8_image_t image;
      pnm_read_image( in_dir+"p3.pnm", image );

      pnm_write_view( out_dir+"p3.pnm", view( image ));
   }

   {
      // a PBMB file
      rgb8_image_t image;
      pnm_read_image( in_dir+"p4.pnm", image );

      pnm_write_view( out_dir+"p4.pnm", view( image ));
   }


   {
      // a PGMB file
      rgb8_image_t image;
      pnm_read_image( in_dir+"p5.pnm", image );

      pnm_write_view( out_dir+"p5.pnm", view( image ));
   }

   {
      // a PBMA file
      rgb8_image_t image;
      pnm_read_image( in_dir+"p6.pnm", image );

      pnm_write_view( out_dir+"p6.pnm", view( image ));
   }
#endif //GIL_NO_PNM
}
