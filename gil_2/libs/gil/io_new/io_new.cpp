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


static void tiff_error_handler(const char *s1, const char *s2, va_list ap)
{
   std::stringstream ss;
   ss << "ERROR: " << s1 << ": ";

   std::vector<char> buffer( 100 );
   vsprintf( &buffer.front(), s2, ap );

   ss.write( &buffer.front(), 100 );
   cout << ss.str() << endl;
}

static void tiff_warning_handler(const char *s1, const char *s2, va_list ap)
{
   stringstream ss;
   ss << "WARNING: " << s1 << ": ";

   std::vector<char> buffer( 100 );
   vsprintf( &buffer.front(), s2, ap );

   ss.write( &buffer.front(), 100 );
   cout << ss.str() << endl;
}

int main()
{
   TIFFSetErrorHandler  ( (TIFFErrorHandler) tiff_error_handler   );
   TIFFSetWarningHandler( (TIFFErrorHandler) tiff_warning_handler );

   // caspian.tif	279x220 64-bit floating point (deflate) Caspian Sea from space

   string file_name( ".\\test_images\\tiff\\libtiffpic\\caspian.tif" );

   basic_tiff_image_read_info info = read_image_info( file_name, tiff_tag() );

   typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
   typedef image< rgb64f_pixel_t, true > rgb64f_planar_image_t;
   typedef rgb64f_planar_image_t::view_t rgb64f_planar_view_t;

   TIFF* img = TIFFOpen( file_name.c_str(), "r" );
   tsize_t scanline_size = TIFFScanlineSize( img );
   tsize_t raster_scanline_size = TIFFRasterScanlineSize( img );

   tsize_t strip_size = TIFFStripSize( img );
   tsize_t max_strips = TIFFNumberOfStrips( img );

   toff_t* stripbytecounts;
   TIFFGetField(img, TIFFTAG_STRIPBYTECOUNTS, &stripbytecounts);

   // a buffer represents three planes for each rows_per_strip
   typedef std::vector< std::vector< double > > vec_t;
   vec_t image( max_strips );
   vec_t::iterator it  = image.begin();
   vec_t::iterator end = image.end();

   for( ; it != end; ++it )
   {
      it->resize( scanline_size );
   }

   for( int stripCount = 0; stripCount < max_strips; stripCount++ )
   {
      if( TIFFReadEncodedStrip( img
                              , stripCount
                              , &image[stripCount].front()
                              , scanline_size ) == -1 )
      {
         cout << "error" << endl;

         break;
      }
   }

   double max = 0;
   double min = 0;
   
   for( int i = 0; i < max_strips; ++i )
   {
      max = std::max( max, *max_element( image[i].begin(), image[i].end() ));
      min = std::min( min, *min_element( image[i].begin(), image[i].end() ));
   }

   return 0;
   
/*
   for( int stripCount = 0; stripCount < max_strips; stripCount++ )
   {
      if( TIFFReadEncodedStrip( img
                              , stripCount
                              , &buffer.front()
                              , -1 ) == -1 )
      {
         cout << "error" << endl;

         break;
      }


      rgb64f_planar_view_t v = planar_rgb_view( image_width
                                              , rows_per_strip
                                              , &buffer[ 0 ]
                                              , &buffer[ scanline_size ]
                                              , &buffer[ 2 * scanline_size ]
                                              , image_width              );
      

      const rgb64f_pixel_t& p = v( 0,0 );

      copy_pixels( v
                 , subimage_view( view( gil_image )
                                , point_t( 0, stripCount * rows_per_strip )
                                , point_t( image_width, rows_per_strip      )));


      const rgb64f_pixel_t& p2 = view( gil_image )( 0,4 );
      int i = 9;

   }

   double max = 0.0;
   double min = 0.0;

   for (int y=0; y<gil_image.height(); ++y)
   {
      rgb64f_planar_view_t::x_iterator src_it = view( gil_image ).row_begin(y);

      for( int x=1; x<gil_image.width()-1; ++x )
      {
         double red = gil::at_c<0>( *src_it );
         double blue = gil::at_c<1>( *src_it );
         double green = gil::at_c<2>( *src_it );

         max = std::max( max, gil::at_c<0>( *src_it ) );
         max = std::max( max, gil::at_c<1>( *src_it ) );
         max = std::max( max, gil::at_c<2>( *src_it ) );

         min = std::min( min, gil::at_c<0>( *src_it ) );
         min = std::min( min, gil::at_c<1>( *src_it ) );
         min = std::min( min, gil::at_c<2>( *src_it ) );
      }
    }
*/

   return 0;
}

/*

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

template<>
void read_tag<string_tag_t>( TIFF*         image
                           , unsigned int  value
                           , string_tag_t& tag   )
{
   std::vector<char> buffer( 100 );

   if( TIFFGetField( image, value, &buffer.front() ) == 1 )
   {
      at_c<1>( tag ).resize( 100 );

      std::copy( buffer.begin()
               , buffer.end()
               , at_c<1>( tag ).begin() );

      at_c<0>( tag ) = true;
   }
   else
   {
      at_c<0>( tag ) = false;
   }
}

template<>
void read_tag<ulong_vec_tag_t>( TIFF*            image
                              , unsigned int     value
                              , ulong_vec_tag_t& tag   )
{
   if( TIFFGetField( image, value, &at_c<1>( tag ).front() ) == 1 )
   {
      at_c<0>( tag ) = true;
   }
   else
   {
      at_c<0>( tag ) = false;
   }
}

template<>
void read_tag<ushort_vec_tag_t>( TIFF*             image
                               , unsigned int      value
                               , ushort_vec_tag_t& tag   )
{
   if( TIFFGetField( image, value, &at_c<1>( tag ).front() ) == 1 )
   {
      at_c<0>( tag ) = true;
   }
   else
   {
      at_c<0>( tag ) = false;
   }
}

template< typename Tag >
bool is_set( const Tag& tag )
{
   return at_c<0>( tag );
}

template< typename Tag >
typename result_of::value_at_c<Tag, 1>::type get_value( const Tag& tag )
{
   return at_c<1>( tag );
}


int _tmain(int argc, _TCHAR* argv[])
{
   TIFFSetErrorHandler  ( (TIFFErrorHandler) tiff_error_handler   );
   TIFFSetWarningHandler( (TIFFErrorHandler) tiff_warning_handler );

   // caspian.tif	279x220 64-bit floating point (deflate) Caspian Sea from space

   TIFF* img = TIFFOpen( ".\\test_images\\tiff\\libtiffpic\\caspian.tif", "r" );

   uint16 buffer[222];
   TIFFGetField( img, TIFFTAG_STRIPBYTECOUNTS, &buffer );


   basic_tiff_image_read_info basic_info;
   read_tag( img, TIFFTAG_IMAGEWIDTH     , basic_info._width             );
   read_tag( img, TIFFTAG_IMAGELENGTH    , basic_info._height            );
   read_tag( img, TIFFTAG_SAMPLESPERPIXEL, basic_info._samples_per_pixel );
   read_tag( img, TIFFTAG_BITSPERSAMPLE  , basic_info._bits_per_sample   );

   // read other information

   tiff_image_read_info info;
   read_tag( img, TIFFTAG_SUBFILETYPE , info._NewSubfileType );
   read_tag( img, TIFFTAG_OSUBFILETYPE, info._SubfileType );
   read_tag( img, TIFFTAG_COMPRESSION , info._Compression );
   read_tag( img, TIFFTAG_PHOTOMETRIC , info._PhotometricInterpretation );

   read_tag( img, TIFFTAG_THRESHHOLDING, info._Threshholding );
   read_tag( img, TIFFTAG_CELLWIDTH, info._CellWidth );
   read_tag( img, TIFFTAG_CELLLENGTH, info._CellLength );
   read_tag( img, TIFFTAG_FILLORDER, info._FillOrder );
   read_tag( img, TIFFTAG_IMAGEDESCRIPTION, info._ImageDescription );
   read_tag( img, TIFFTAG_MAKE, info._Make );
   read_tag( img, TIFFTAG_MODEL, info._Model );

   read_tag( img, TIFFTAG_ORIENTATION, info._Orientation );

   read_tag( img, TIFFTAG_ROWSPERSTRIP, info._RowsPerStrip );

   read_tag( img, TIFFTAG_XRESOLUTION, info._XResolution );
   read_tag( img, TIFFTAG_YRESOLUTION, info._YResolution );

   read_tag( img, TIFFTAG_PLANARCONFIG, info._PlanarConfiguration );

   read_tag( img, TIFFTAG_GRAYRESPONSEUNIT, info._GrayResponseUnit );

   read_tag( img, TIFFTAG_RESOLUTIONUNIT, info._ResolutionUnit );

   read_tag( img, TIFFTAG_SOFTWARE, info._Software );
   read_tag( img, TIFFTAG_DATETIME, info._DateTime );
   read_tag( img, TIFFTAG_ARTIST, info._Artist );
   read_tag( img, TIFFTAG_HOSTCOMPUTER, info._HostComputer );

   read_tag( img, TIFFTAG_COPYRIGHT, info._Copyright );



   // read buffers

   if(  is_set( basic_info._width  ) 
     && is_set( basic_info._height )
     && is_set( info._RowsPerStrip ))
   {
      float image_length   = static_cast<float>( get_value( basic_info._width )) 
                           * static_cast<float>( get_value( basic_info._samples_per_pixel ));

      float rows_per_strip = static_cast<float>( get_value( info._RowsPerStrip ));

      unsigned int strips_per_image = floor( image_length + rows_per_strip - 1.f ) / rows_per_strip;

      at_c<1>( info._StripOffsets    ).resize( strips_per_image );
      at_c<1>( info._StripByteCounts ).resize( strips_per_image );

      read_tag( img, TIFFTAG_STRIPOFFSETS, info._StripOffsets );
      read_tag( img, TIFFTAG_STRIPBYTECOUNTS, info._StripByteCounts );
   }


   read_tag( img, TIFFTAG_MINSAMPLEVALUE, info._MinSampleValue );
   read_tag( img, TIFFTAG_MAXSAMPLEVALUE, info._MaxSampleValue );

   read_tag( img, TIFFTAG_FREEOFFSETS, info._FreeOffsets );
   read_tag( img, TIFFTAG_FREEBYTECOUNTS, info._FreeByteCounts );

   read_tag( img, TIFFTAG_GRAYRESPONSECURVE, info._GrayResponseCurve );

   read_tag( img, TIFFTAG_COLORMAP, info._ColorMap );

   read_tag( img, TIFFTAG_EXTRASAMPLES, info._ExtraSamples );

	return 0;
}
*/