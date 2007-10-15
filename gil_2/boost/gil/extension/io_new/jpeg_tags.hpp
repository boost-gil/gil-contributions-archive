/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef JPEG_TAGS_HPP
#define JPEG_TAGS_HPP

/// \file
/// \brief All supported jpeg tags by the gil io extension.
///
/// \author Christian Henning
///         

namespace boost { namespace gil {

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

} // namespace gil
} // namespace boost

#endif // TIFF_TAGS_HPP
