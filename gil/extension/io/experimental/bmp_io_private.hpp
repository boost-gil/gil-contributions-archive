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
/// \date   2005-2006 \n Last updated December 11, 2006

#include <stdio.h>
#include <boost/static_assert.hpp>
#include <boost/scoped_array.hpp>
#include <vector>
#include "../../core/gil_all.hpp"
#include "io_error.hpp"

ADOBE_GIL_NAMESPACE_BEGIN

namespace detail {

/// BMP compression types
enum {
	ct_rgb		= 0,	///< RGB without compression
	ct_rle8		= 1,	///< 8 bit index with RLE compression
	ct_rle4		= 2,	///< 4 bit index with RLE compression
	ct_bitfield	= 3		///< 16 or 32 bit fields without compression
};

/// BMP file signature
enum {
	bm_signature = 0x4D42
};

/// BMP file header
struct file_header {
	int			type;	///< File type
	long		size;	///< File size in bytes
	long		offset;	///< Pixels file offset
};

/// BMP information (Windows) or core (OS2) header
struct info_header {
	int			what;	///< Compression type
	int			width;	///< Width in pixels
	int			height;	///< Height in pixels, negative for top-down
	int			planes;	///< Bit planes, always 1
	int			bpp;	///< Bits per pixel
	int			colors;	///< Number of colors in the palette, zero for all colors
	int			entry;	///< Size of pallete entry in bytes
};

/// BMP color palette
struct color_map {
	unsigned	blue;	///< Blue bits mask
	unsigned	green;	///< Green bits mask
	unsigned	red;	///< Red bits mask
	unsigned	unused;	///< Reserved
};

/// Color channel mask
struct bit_field {
	unsigned	mask;	///< Bit mask at corresponding position
	unsigned	width;	///< Bit width of the mask
	unsigned	shift;	///< Bit position from right to left
};

/// BMP color masks
struct color_mask {
	bit_field	red;	 ///< Red bits
	bit_field	green; ///< Green bits
	bit_field	blue;	 ///< Blue bits
};

/// Determines whether the given channel width and color space are supported for reading and writing
template <typename Chn, typename Spc> struct bmp_read_write_support_private {
	enum {
		supported	= false,
		channel		= 0,
		pixel		= 0
	};
};
template <> struct bmp_read_write_support_private<bits8, gray_t> {
	enum {
		supported	= true,
		channel		= 8,
		pixel		= 8
	};
};
template <> struct bmp_read_write_support_private<bits8, rgb_t> {
	enum {
		supported	= true,
		channel		= 8,
		pixel		= 24
	};
};
template <> struct bmp_read_write_support_private<bits8, rgba_t> {
	enum {
		supported	= true,
		channel		= 8,
		pixel		= 32
	};
};

struct indexed_pixel_deref_fn
{
   typedef indexed_pixel_deref_fn const_t;
   typedef rgb8_pixel_t value_type;
   typedef value_type reference;   // returns by value!
   typedef value_type const_reference;
   typedef reference result_type;

   typedef rgb8_pixel_t value_type;
   static const bool is_mutable = false;

   typedef std::vector< rgb8_pixel_t > colors_t;

   indexed_pixel_deref_fn( const colors_t& table ) : _table( table ) {}

   rgb8_pixel_t operator()( const gray8_pixel_t& index ) const
   {
      return _table[index[0]];
   }

   const colors_t& _table;
};


template <typename DstP, typename CC=default_color_converter >
class color_converter_unary {
private:
   CC _cc;                     // color-converter
public:
   color_converter_unary() {}
   color_converter_unary(CC cc_in) : _cc(cc_in) {}

   template <typename SrcP>
   DstP operator()(const SrcP& srcP) const {
       DstP dstP;
       _cc(srcP,dstP);
       return dstP;
   }
};

/// Transfers and converts row of pixels
template <typename V, typename C> struct transfer {
	typedef typename V::x_iterator iterator_t;
	typedef typename V::pixel_t    pixel_t;
	typedef typename V::channel_t  channel_t;

	/// From BMP to GIL
	static void convert(int bpp, const byte_t *src, iterator_t dest, int cnt, const color_map pal[], const color_mask& msk) throw() {
		unsigned bit;
		byte_t   pak, idx;

		switch (bpp)
		{
		case 1:
			// 1 indexed
			for (bit = 0; cnt > 0; --cnt, ++dest) {
				if (bit == 0) {
					bit = 8;
					pak = *src++;
				}
				idx = (pak >> --bit) & 0x01;

				*dest = io::convertor<V, C>::make(pal[idx].red, pal[idx].green, pal[idx].blue);
			}
			break;

		case 4:
			// 4 indexed
			for (bit = 0; cnt > 0; --cnt, ++dest) {
				if (bit == 0) {
					bit = 8;
					pak = *src++;
				}
				bit -= 4;
				idx = (pak >> bit) & 0x0F;

				*dest = io::convertor<V, C>::make(pal[idx].red, pal[idx].green, pal[idx].blue);
			}
			break;

		case 8:
			// 8 indexed
			for (; cnt > 0; --cnt, ++src, ++dest) {
				idx = *src;
				*dest = io::convertor<V, C>::make(pal[idx].red, pal[idx].green, pal[idx].blue);
			}
			break;

		case 15:
		case 16:
			// 5-5-5, 5-6-5 BGR
			for (; cnt > 0; --cnt, ++dest, src += 2) {
				int p = (src[1] << 8) | src[0];

				int r = ((p & msk.red.mask)   >> msk.red.shift)   << (8 - msk.red.width);
				int g = ((p & msk.green.mask) >> msk.green.shift) << (8 - msk.green.width);
				int b = ((p & msk.blue.mask)  >> msk.blue.shift)  << (8 - msk.blue.width);

				*dest = io::convertor<V, C>::make(r, g, b);
			}
			break;

		case 24:
			// 8-8-8 BGR
			for (; cnt > 0; --cnt, ++dest) {
				byte_t b = *src++;
				byte_t g = *src++;
				byte_t r = *src++;

				*dest = io::convertor<V, C>::make(r, g, b);
			}
			break;

		case 32:
			// 8-8-8-8 BGR*
			for (; cnt > 0; --cnt, ++dest) {
				byte_t b = *src++;
				byte_t g = *src++;
				byte_t r = *src++;
				byte_t a = *src++;

				*dest = io::convertor<V, C>::make(r, g, b);
			}
			break;
		}
	}

	/// From GIL to BMP
	static void convert(int bpp, iterator_t src, byte_t *dest, int cnt) throw() {
		channel_t r, g, b, a;

		switch (bpp)
		{
		case 8:
			// 8
			for (; cnt > 0; --cnt, ++src, ++dest) {
				io::convertor<V, C>::split(*src, r, g, b, a);
				*dest = g;
			}
			break;

		case 24:
			// 8-8-8
			for (; cnt > 0; --cnt, ++src) {
				io::convertor<V, C>::split(*src, r, g, b, a);

				*dest++ = b;
				*dest++ = g;
				*dest++ = r;
			}
			break;

		case 32:
			// 8-8-8-8
			for (; cnt > 0; --cnt, ++src) {
				io::convertor<V, C>::split(*src, r, g, b, a);

				*dest++ = b;
				*dest++ = g;
				*dest++ = r;
				*dest++ = 0;
			}
			break;
		}
	}
};



class bmp_reader : public file_mgr {
protected:
   info_header _info_header;

    void init() {

      const int header     = 14;
      const int info_win32 = 40;
      const int info_os2   = 12;

      // Read file header.
      file_header header;
      header.type = read_int16();
      header.size = read_int32();
      read_int16(); // read reserved bytes
      read_int16(); // read reserved bytes
      header.offset = read_int32();

      if (header.type != bm_signature) {
	      io_error("file_mgr: not a BMP file");
      }

      if (header.offset >= header.size) {
	      io_error("file_mgr: invalid BMP file header");
      }

      // Read the info header size.
      int info_header_size = read_int32();

      if( strc == 40 )
      {
	      // Windows header
	      _info_header.width  = read_int32();
	      _info_header.height = read_int32();
	      _info_header.planes = read_int16();
	      _info_header.bpp    = read_int16();
	      _info_header.what   = read_int32();
					         read_int32();
					         read_int32();
					         read_int32();
	      _info_header.colors = read_int32();
					         read_int32();
	      _info_header.entry  = 4;
      }
      else if (strc == 12) {
	      // OS2 header
	      _info_header.width  = read_int16();
	      _info_header.height = read_int16();
	      _info_header.planes = read_int16();
	      _info_header.bpp    = read_int16();
	      _info_header.what   = ct_rgb;
	      _info_header.colors = 0;
	      _info_header.entry  = 3;
      }
      else {
	      io_error("file_mgr: invalid BMP info header");
      }

      /// check supported bits per pixel
      if (_info_header.bpp < 1 || _info_header.bpp > 32) {
	      io_error("file_mgr: unsupported BMP format");
      }
    }


public:
    bmp_reader(FILE* file)           : file_mgr(file)           { init(); }
    bmp_reader(const char* filename) : file_mgr(filename, "rb") { init(); }

   template <typename VIEW>
   void apply( const VIEW& view )
   {
      io_error_if( view.dimensions() != get_dimensions()
                 , "bmp_reader::apply(): input view dimensions do not match the image file");

      // read the color masks
      if( _info_header.what == ct_bitfield )
      {
         mask.red.mask    = read_int32();
         mask.green.mask  = read_int32();
         mask.blue.mask   = read_int32();

         mask.red.width   = io::count_ones( mask.red.mask   );
         mask.green.width = io::count_ones( mask.green.mask );
         mask.blue.width  = io::count_ones( mask.blue.mask  );

         mask.red.shift   = io::trailing_zeros( mask.red.mask   );
         mask.green.shift = io::trailing_zeros( mask.green.mask );
         mask.blue.shift  = io::trailing_zeros( mask.blue.mask  );
      }
      else if( _info_header.what == ct_rgb )
      {
         switch( _info_header.bpp )
         {
            case 15: case 16:
            {
		         mask.red.mask   = 0x007C00; mask.red.width   = 5; mask.red.shift   = 10;
		         mask.green.mask = 0x0003E0; mask.green.width = 5; mask.green.shift =  5;
		         mask.blue.mask  = 0x00001F; mask.blue.width  = 5; mask.blue.shift  =  0;
		         break;
            }

	         case 24: case 32:
	         {
		         mask.red.mask   = 0xFF0000; mask.red.width   = 8; mask.red.shift   = 16;
		         mask.green.mask = 0x00FF00; mask.green.width = 8; mask.green.shift =  8;
		         mask.blue.mask  = 0x0000FF; mask.blue.width  = 8; mask.blue.shift  =  0;
		         break;
            }
	      }
      }
      else
      {
	      io_error( "bmp_reader::apply(): unsupported BMP compression" );
      }

      // Read the color map.

		std::vector<color_map> palette;

      if( _info_header.bpp <= 8 )
      {
	      int entries = _info_header.colors;

	      if( entries == 0 )
	      {
		      entries = 1 << _info_header.bpp;
	      }

         palette.resize( entries );

	      for( int i = 0; i < entries; ++i )
	      {
		      palette[i].blue  = read_int8();
		      palette[i].green = read_int8();
		      palette[i].red   = read_int8();

		      if( _info_header.entry > 3 )
		      {
			      read_int8();
		      }
	      }
      }

      seek(header.offset);

      // the row pitch must be multiple 4 bytes
      int pitch;

      if (_info_header.bpp < 8) {
	      pitch = ((_info_header.width * _info_header.bpp) + 7) >> 3;
      }
      else {
	      pitch = _info_header.width * ((_info_header.bpp + 7) >> 3);
      }
      pitch = (pitch + 3) & ~3;

      // read the raster
      std::vector<byte_t> row(pitch);

      int ybeg = 0;
      int yend = _info_header.height;
      int yinc = 1;

      if( _info_header.height > 0 )
      {
	      ybeg = _info_header.height - 1;
	      yend = -1;
	      yinc = -1;
      }

		const color_map *pal = 0;

		if( palette.size() > 0 )
		{
			pal = &palette.front();
		}

      for( int y = ybeg; y != yend; y += yinc )
      {
	      read(&row.front(), pitch);
	      transfer<V, Spc>::convert(_info_header.bpp, &row.front(), view.row_begin(y), _info_header.width, pal, mask);
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

    }
};

class bmp_writer : public file_mgr {

public:
    bmp_writer(FILE* file)           : file_mgr(file)           {}
    bmp_writer(const char* filename) : file_mgr(filename, "wb") {}
    
    template <typename VIEW>
    void apply(const VIEW& view) {

    }
};

} // namespace detail

ADOBE_GIL_NAMESPACE_END

#endif
