/*
  Copyright 2005-2006 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_BMP_IO_PRIVATE_H
#define GIL_BMP_IO_PRIVATE_H

/// \file
/// \brief  Internal support for reading and writing BMP files
/// \author Christian Henning
/// \date   2005-2006 \n Last updated November 06, 2006

#include <stdio.h>
#include <boost/static_assert.hpp>
#include <boost/scoped_array.hpp>
#include <vector>
#include "../../core/gil_all.hpp"
#include "io_error.hpp"

ADOBE_GIL_NAMESPACE_BEGIN

namespace detail {

// TODO chh: define bmp_read_support_private structs


#pragma pack(push,2)
struct gil_bitmap_file_header
{
   unsigned short  _bfType; 
   unsigned long   _bfSize; 
   unsigned short  _bfReserved1; 
   unsigned short  _bfReserved2; 
   unsigned long   _bfOffBits; 
};

struct gil_bitmap_info_header
{
   unsigned long  _biSize;
   long           _biWidth;
   long           _biHeight;
   unsigned short _biPlanes;
   unsigned short _biBitCount;
   unsigned long  _biCompression;
   unsigned long  _biSizeImage;
   long           _biXPelsPerMeter;
   long           _biYPelsPerMeter;
   unsigned long  _biClrUsed;
   unsigned long  _biClrImportant;
};
#pragma pack(pop)


class bmp_reader : public file_mgr {
protected:
   gil_bitmap_info_header _info_header;

    void init() {
      // Read file and info header.
      gil_bitmap_file_header file_header;

      BOOST_STATIC_ASSERT( sizeof( gil_bitmap_file_header ) == 14 );
      BOOST_STATIC_ASSERT( sizeof( gil_bitmap_info_header ) == 40 );

      io_error_if( fread( &file_header, sizeof( char ), sizeof( gil_bitmap_file_header ), get() ) == 0 
                 , "file_mgr: failed to read file" );

      io_error_if( file_header._bfType != 0x4D42
                 , "file_mgr: this is not a bitmap file" );

      io_error_if( fread( &_info_header, sizeof( char ), sizeof( gil_bitmap_info_header ), get() ) == 0 
                 , "file_mgr: failed to read file" );
    }


public:
    bmp_reader(FILE* file)           : file_mgr(file)           { init(); }
    bmp_reader(const char* filename) : file_mgr(filename, "rb") { init(); }

    template <typename VIEW>
    void apply(const VIEW& view) {
      io_error_if( _info_header._biBitCount    != 24
                 , "bmp_reader::apply(): Only 24bit images are supported." );
      io_error_if( _info_header._biClrUsed     != 0
                 , "bmp_reader::apply(): no indexed images are supported, yet." );
      io_error_if( _info_header._biCompression != 0L // only BI_RGB is supported
                 , "bmp_reader::apply(): only RGB images are supported." );
      io_error_if( view.dimensions() != get_dimensions()
                 , "bmp_reader::apply(): input view dimensions do not match the image file");

      const int nNumChannels = VIEW::color_space_t::num_channels;

      unsigned int nScanline = view.width() * nNumChannels;
      unsigned int nPadding  = sizeof( unsigned long ) - view.width() % sizeof( unsigned long );

      boost::scoped_array<unsigned char> spScanlineBuffer( new unsigned char[ nScanline ] );

      typedef typename bgr8_image_t::pixel_t pixel_t;

      // Read image pixels, without the padding bytes.
      // The rgb channels are stored in bgr order. Also, the image is flipped upside down.
      for( int y=view.height(); y > 0; --y )
      {
         io_error_if( fread( spScanlineBuffer.get(), sizeof( char ), nScanline, get() ) == 0 
                    , "file_mgr: failed to read file" );

         pixel_t* pPixels = reinterpret_cast<pixel_t*>( spScanlineBuffer.get() );
         std::copy( pPixels, pPixels + view.width(), view.row_begin( y-1 ) );

         if( nPadding )
            fseek( get(), nPadding, SEEK_CUR );
      }
    }
    
    template <typename IMAGE>
    void read_image(IMAGE& im) {
        resize_clobber_image(im,get_dimensions());
        apply(view(im));
    }

    point2<int> get_dimensions() const {
        return point2<int>( _info_header._biWidth,_info_header._biHeight );
    }
};


//  Used when user want to read and convert image at the same time.
class bmp_reader_color_convert : public bmp_reader {
public:
    bmp_reader_color_convert(FILE* file)           : bmp_reader(file) {}
    bmp_reader_color_convert(const char* filename) : bmp_reader(filename) {}

    template <typename VIEW>
    void apply(const VIEW& view) {

        if( _info_header.biCompression != BI_RGB )
        {
           io_error("bmp_reader_color_covert::apply(): No compression is supported.");
        }

        switch (_info_header.biBitCount) {
        case 0: {
            // see msdn: Windows 98/Me, Windows 2000/XP: The number of bits-per-pixel is specified or is implied by the JPEG or PNG format.
            io_error("bmp_reader_color_covert::apply(): unknown color type");
            break;
        }

        case 1: {
            // see msdn: The bitmap has 2 colors.
            io_error("bmp_reader_color_covert::apply(): unknown color type");
            break;
        }
        case 4: {
            // see msdn: The bitmap has a maximum of 16 colors.
            io_error("bmp_reader_color_covert::apply(): unknown color type");
            break;
        }
        case 8: {
            // see msdn: The bitmap has a maximum of 256 colors.

            io_error("bmp_reader_color_covert::apply(): no indexed images are supported.");
            break;
        }
        case 16: {
            // see msdn: The bitmap has a maximum of 2^16 colors.
            io_error("bmp_reader_color_covert::apply(): unknown color type");
            break;
        }
        case 24: {
            // see msdn: The bitmap has a maximum of 2^24 colors.
            const int nNumChannels = VIEW::color_space_t::num_channels;

            unsigned int nScanline = view.width() * nNumChannels;
            unsigned int nPadding  = view.width() % sizeof( unsigned long );

            boost::scoped_array<unsigned char> spScanlineBuffer( new unsigned char[ nScanline ] );

            typedef typename VIEW::pixel_t pixel_t;
            for( int y=0; y < view.height(); ++y )
            {
               io_error_if( fread( spScanlineBuffer.get(), sizeof( char ), nScanline, get() ) == 0 
                        , "file_mgr: failed to read file" );

               pixel_t* pPixels = reinterpret_cast<pixel_t*>( spScanlineBuffer.get() );

               std::transform( pPixels, pPixels + view.width(), view.row_begin(y),
                               color_converter<pixel_t>());

               if( nPadding )
                  fseek( get(), nPadding, SEEK_CUR );
            }

            break;
        }
        case 32: {
            // see msdn: The bitmap has a maximum of 2^32 colors.
            // TODO chh: Are the row still padded, even in 32 bit mode?

            io_error("bmp_reader_color_covert::apply(): no indexed images are supported.");
            break;
        }
        default:
            io_error("bmp_reader_color_covert::apply(): unknown color type");
        }
    }    

    template <typename IMAGE>
    void read_image(IMAGE& im) {
        resize_clobber_image(im,get_dimensions());
        apply(view(im));
    }
};


class bmp_writer : public file_mgr {

public:
    bmp_writer(FILE* file)           : file_mgr(file)           {}
    bmp_writer(const char* filename) : file_mgr(filename, "wb") {}
    
    template <typename VIEW>
    void apply(const VIEW& view) {

      const int nNumChannels = VIEW::color_space_t::num_channels;

      unsigned int nImageSize = view.width() 
                              * view.height() 
                              * VIEW::color_space_t::num_channels;
      // Write the file header.
      gil_bitmap_file_header file_header;
      file_header._bfType = 0x4D42;
      file_header._bfSize = sizeof( gil_bitmap_file_header )
                         + sizeof( gil_bitmap_info_header )
                         + nImageSize;

      file_header._bfReserved1 = 0;
      file_header._bfReserved2 = 0;
      file_header._bfOffBits = sizeof( gil_bitmap_file_header )
                            + sizeof( gil_bitmap_info_header );


      io_error_if( fwrite( &file_header, sizeof(char), sizeof( gil_bitmap_file_header ), get() ) == 0
                 , "file_mgr: failed to write file" );

      // Write the info header.
      gil_bitmap_info_header info_header;

      info_header._biSize = sizeof( gil_bitmap_info_header );
      info_header._biWidth  = view.width();
      info_header._biHeight = view.height();
      info_header._biPlanes = 1;
      info_header._biBitCount = nNumChannels * 8;
      info_header._biCompression  = 0L; // only BI_RGB is supported
      info_header._biSizeImage = nImageSize;
      info_header._biXPelsPerMeter = 0;
      info_header._biYPelsPerMeter = 0;
      info_header._biClrUsed       = 0;
      info_header._biClrImportant  = 0;

      io_error_if( fwrite( &info_header, sizeof(char), sizeof( gil_bitmap_info_header ), get() ) == 0 
                 , "file_mgr: failed to write file"                                              );

      unsigned int nScanline = view.width() * nNumChannels;
      unsigned int nPadding  = view.width() % sizeof( unsigned long );

      std::vector<bgr8_pixel_t> bgr_row( view.width() );

      // Write the pixels. Scanlines must be DWORD aligned.
      // The rgb channels are stored in bgr order. Also, the image is flipped upside down.
      VIEW store_view = flipped_up_down_view( view );

      for( int y=0;y<view.height(); ++y ) {

         std::transform( store_view.row_begin(y), store_view.row_end(y), bgr_row.begin(),
                           color_converter<bgr8_pixel_t>());

         io_error_if( fwrite( &bgr_row.front(), sizeof(char), nScanline, get() ) == 0
                  , "file_mgr: failed to write file"                                );

         for( unsigned int i = 0; i < nPadding; ++i ) {
            io_error_if( fputc( 0, get() ) == EOF
                       , "file_mgr: failed to write file" );
         }
      }
    }
};

} // namespace detail

ADOBE_GIL_NAMESPACE_END

#endif
