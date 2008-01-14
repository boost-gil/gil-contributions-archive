/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_TAGS_HPP_INCLUDED 
#define BOOST_GIL_EXTENSION_IO_JPEG_TAGS_HPP_INCLUDED

#include <jpeglib.h>
#include <boost/gil/extension/io_new_2/detail/base.hpp>
/// \file
/// \brief All supported jpeg tags by the gil io extension.
///
/// \author Christian Henning
///         

namespace boost { namespace gil {

struct jpeg_tag : format_tag {};

struct jpeg_image_width
{
   typedef JDIMENSION type;
};

struct jpeg_image_height
{
   typedef JDIMENSION type;
};

struct jpeg_num_components
{
   typedef int type;
};

struct jpeg_color_space
{
   typedef J_COLOR_SPACE type;
};

struct jpeg_quality
{
   typedef int type;
};

template<>
struct image_read_info<jpeg_tag> 
{
   jpeg_image_width::type  _width;
   jpeg_image_height::type _height;

   jpeg_num_components::type _num_components;
   jpeg_color_space::type    _color_space;
};

template<>
struct image_write_info<jpeg_tag>
{
   jpeg_quality::type _quality;
};


} // namespace gil
} // namespace boost

#endif // TIFF_TAGS_HPP
