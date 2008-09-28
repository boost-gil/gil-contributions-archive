/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_TAGS_HPP_INCLUDED 
#define BOOST_GIL_EXTENSION_IO_BMP_TAGS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include "detail/base.hpp"

namespace boost { namespace gil {

struct bmp_tag : format_tag {};

template< Property > 
struct bmp_property_base
{
    typedef Property type;
}

// The bitmap width in pixels (signed integer).
struct bmp_image_width : bmp_property_base< bits32s > {};

// The bitmap height in pixels (signed integer).
struct bmp_image_height : bmp_property_base< bits32s > {};

// The number of bits per pixel, which is the color depth of the image.
// Typical values are 1, 4, 8, 16, 24 and 32.
struct bmp_bits_per_pixel : bmp_property_base< bits16 > {};

// The compression method being used.
template< int Compression > struct compression_type : public mpl::false_ {};
template<> struct compression_type< 0 > : public mpl::int_< 0 > {}; // RGB without compression
template<> struct compression_type< 1 > : public mpl::int_< 1 > {}; // 8 bit index with RLE compression
template<> struct compression_type< 2 > : public mpl::int_< 2 > {}; // 4 bit index with RLE compression
template<> struct compression_type< 3 > : public mpl::int_< 3 > {}; // 16 or 32 bit fields without compression

struct bmp_compression : bmp_property_base< bits32 > {};

// The image size. This is the size of the raw bitmap data (see below), 
// and should not be confused with the file size.
struct bmp_image_size : bmp_property_base< bits32 > {};

// The horizontal resolution of the image. (pixel per meter, signed integer)
struct bmp_horizontal_resolution : bmp_property_base< bits32s > {};

// The vertical resolution of the image. (pixel per meter, signed integer)
struct bmp_vertical_resolution : bmp_property_base< bits32s > {};

// The number of colors in the color palette, or 0 to default to 2^n - 1.
struct bmp_num_colors : bmp_property_base< bits32 > {};


// The number of important colors used, or 0 when every color is important; 
// generally ignored.
struct bmp_num_important_colors : bmp_property_base< bits32 > {};


template<>
struct image_read_info< bmp_tag >
{
    bmp_image_width::type  _width;
    bmp_image_height::type _height;

    bmp_bits_per_pixel::type _bits_per_pixel;

    bmp_compression::type _compression;

    bmp_image_size::type _image_size;

    bmp_horizontal_resolution::type _horizontal_resolution;
    bmp_vertical_resolution::type   _vertical_resolution;

    bmp_num_colors::type            _num_colors;
    bmp_num_important_colors::type  _num_important_colors;
};


template<>
struct image_read_settings< bmp_tag > : public image_read_settings_base
{
    image_read_settings< bmp_tag >()
    : image_read_settings_base()
    {}

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    {}
};


template<>
struct image_write_info< bmp_tag >
{
};


} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_TAGS_HPP_INCLUDED
