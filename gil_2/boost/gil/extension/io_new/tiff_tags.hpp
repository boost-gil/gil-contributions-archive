/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_TAGS_HPP
#define TIFF_TAGS_HPP

/// \file
/// \brief All supported tiff tags by the gil io extension.
///
/// For a description see: http://www.awaresystems.be/imaging/tiff/tifftags/baseline.html
///
/// \author Christian Henning
///         

/// see http://www.remotesensing.org/libtiff/
/// or http://www.libtiff.org/man/


extern "C" {
#include "tiff.h"
}

namespace boost { namespace gil {

struct tiff_new_subfile_type
{
   typedef boost::uint32_t type;
   static const tag = TIFFTAG_SUBFILETYPE;
};

struct tiff_subfile_type
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_OSUBFILETYPE;
};

struct tiff_image_width
{
   typedef boost::uint32_t type;
   static const tag = TIFFTAG_IMAGEWIDTH;
};

struct tiff_image_height
{
   typedef boost::uint32_t type;
   static const tag = TIFFTAG_IMAGELENGTH;
};

struct tiff_bits_per_sample
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_BITSPERSAMPLE;
};

struct tiff_compression
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_COMPRESSION;
};

struct tiff_photometric_interpretation
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_PHOTOMETRIC;
};

struct tiff_thresholding
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_THRESHHOLDING;
};

struct tiff_cell_width
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_CELLWIDTH;
};

struct tiff_cell_length
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_CELLLENGTH;
};

struct tiff_fill_order
{
   typedef std::string type;
   static const tag = TIFFTAG_FILLORDER;
};

struct tiff_image_description
{
   typedef std::string type;
   static const tag = TIFFTAG_IMAGEDESCRIPTION;
};

struct tiff_make
{
   typedef std::string type;
   static const tag = TIFFTAG_MAKE;
};

struct tiff_model
{
   typedef std::string type;
   static const tag = TIFFTAG_MODEL;
};

struct tiff_orientation
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_ORIENTATION;
};

struct tiff_samples_per_pixel
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_SAMPLESPERPIXEL;
};

struct tiff_rows_per_strip
{
   typedef boost::uint32_t type;
   static const tag = TIFFTAG_ROWSPERSTRIP;
};

struct tiff_min_sample_value
{
   typedef boost::uint16_t* type;
   static const tag = TIFFTAG_XRESOLUTION;
};

struct tiff_max_sample_value
{
   typedef boost::uint16_t* type;
   static const tag = TIFFTAG_YRESOLUTION;
};

struct tiff_x_resolution
{
   typedef float type;
   static const tag = TIFFTAG_XRESOLUTION;
};

struct tiff_y_resolution
{
   typedef float type;
   static const tag = TIFFTAG_YRESOLUTION;
};

struct tiff_planar_configuration
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_PLANARCONFIG;
};

struct tiff_gray_response_unit
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_GRAYRESPONSEUNIT;
};

struct tiff_gray_response_curve
{
   typedef boost::uint16_t* type;
   static const tag = TIFFTAG_GRAYRESPONSECURVE;
};

struct tiff_resolution_unit
{
   typedef boost::uint16_t type;
   static const tag = TIFFTAG_RESOLUTIONUNIT;
};

struct tiff_software
{
   typedef std::string type;
   static const tag = TIFFTAG_SOFTWARE;
};

struct tiff_date_time
{
   typedef std::string type;
   static const tag = TIFFTAG_DATETIME;
};

struct tiff_artist
{
   typedef std::string type;
   static const tag = TIFFTAG_ARTIST;
};

struct tiff_host_computer
{
   typedef std::string type;
   static const tag = TIFFTAG_HOSTCOMPUTER;
};

struct tiff_color_map
{
   typedef boost::uint16_t* type;
   static const tag = TIFFTAG_COLORMAP;
};

struct tiff_extra_samples
{
   typedef boost::uint16_t* type;
   static const tag = TIFFTAG_EXTRASAMPLES;
};

struct tiff_copyright
{
   typedef std::string type;
   static const tag = TIFFTAG_COPYRIGHT;
};

} // namespace gil
} // namespace boost

#endif // TIFF_TAGS_HPP
