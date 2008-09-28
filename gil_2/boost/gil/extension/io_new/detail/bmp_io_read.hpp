/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include "base.hpp"
#include "reader_base.hpp"
#include "io_device.hpp"
#include "typedefs.hpp"
#include "jpeg_supported_types.hpp"

namespace boost { namespace gil { namespace detail {

static const int header_size     = 14;
static const int win32_info_size = 40;
static const int os2_info_size   = 12;
static const int bm_signature    = 0x4D42;

template< int Compression > struct compression_type : public mpl::false_ {};
template<> struct compression_type< 0 > : public mpl::int_< 0 > {}; // RGB without compression
template<> struct compression_type< 1 > : public mpl::int_< 1 > {}; // 8 bit index with RLE compression
template<> struct compression_type< 2 > : public mpl::int_< 2 > {}; // 4 bit index with RLE compression
template<> struct compression_type< 3 > : public mpl::int_< 3 > {}; // 16 or 32 bit fields without compression

/// BMP file header
struct file_header
{
    bits16 type;   // File type
    bits32 size;   // File size in bytes
    bits32 offset; // Pixels file offset
};

/// BMP information (Windows) or core (OS2) header
struct info_header
{
    bits32  size;    // the size of this header (40 bytes)
    bits32s width;   // the bitmap width in pixels (signed integer).
    bits32s width;   // the bitmap height in pixels (signed integer).
    bits16 num_color_planes; // the number of color planes being used. Must be set to 1.
    bits16 num_color_planes; // the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    bits32 compression; // the compression method being used. See the next table for a list of possible values.
    bits32 image_size;  // the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    bits32s horizontal_resolution; // the horizontal resolution of the image. (pixel per meter, signed integer)
    bits32  vertical_resolution; // the vertical resolution of the image. (pixel per meter, signed integer)
    bits32  num_colors; // the number of colors in the color palette, or 0 to default to 2n.
    bits32 num_important_colors; //the number of important colors used, or 0 when every color is important; generally ignored.
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


template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , bmp_tag
            , ConversionPolicy
            > 
    : public reader_base< jpeg_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:
    reader( Device& device )
    {}

    reader( Device&     device
          , const cc_t& cc
          )
    : reader_base< bmp_tag
                 , ConversionPolicy
                 >( cc )
    {}

    image_read_info< bmp_tag > get_info()
    {
        image_read_info< bmp_tag > ret;
        return ret;
    }

    template<typename View>
    void apply( const View& view )
    {
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_READ_HPP_INCLUDED
