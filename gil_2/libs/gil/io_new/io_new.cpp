// io_new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "boost/gil/extension/io_new/tiff_io.hpp"

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

using namespace std;
using namespace boost;
using namespace gil;
using namespace fusion;

template< typename Tag >
void read_tag( TIFF*        image
             , unsigned int value
             , Tag&         tag   )
{
   if( TIFFGetField( image, value, &at_c<1>( tag ) ) == 1 )
   {
      at_c<0>( tag ) = true;
   }
   else
   {
      at_c<0>( tag ) = false;
   }
}


int _tmain(int argc, _TCHAR* argv[])
{
   // caspian.tif	279x220 64-bit floating point (deflate) Caspian Sea from space
   
   TIFF* img = TIFFOpen( ".\\test_images\\tiff\\libtiffpic\\caspian.tif", "r" );

   tiff_image_read_info info;


   read_tag( img, TIFFTAG_SUBFILETYPE , info._NewSubfileType );
   read_tag( img, TIFFTAG_OSUBFILETYPE, info._SubfileType );
   read_tag( img, TIFFTAG_COMPRESSION , info._Compression );
   read_tag( img, TIFFTAG_PHOTOMETRIC , info._PhotometricInterpretation );

	return 0;
}
