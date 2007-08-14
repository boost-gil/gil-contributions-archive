/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_TIFF_IO_PRIVATE_HPP
#define GIL_TIFF_IO_PRIVATE_HPP

/// \file
/// \brief 
//
/// \author Christian Henning
/// 

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

#include <algorithm>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>

namespace boost { namespace gil {

struct tiff_tag {};

typedef boost::shared_ptr<TIFF> tiff_file_t;

} // namespace gil
} // namespace boost


#include <boost/gil/extension/io_new/base.hpp>
#include <boost/gil/extension/io_new/tiff_tags.hpp>

namespace boost { namespace gil {

struct basic_tiff_image_read_info
{
   tiff_image_width::type  _width;
   tiff_image_height::type _height;

   tiff_bits_per_sample::type   _bits_per_sample;
   tiff_samples_per_pixel::type _samples_per_pixel;
   tiff_sample_format::type     _sample_format;

   tiff_planar_configuration::type _planar_configuration;

   tiff_photometric_interpretation::type _photometric_interpretation;
};

struct tiff_image_write_info
{
};

} // namespace gil
} // namespace boost

#include <boost/gil/extension/io_new/tiff_io_private.hpp>

namespace boost { namespace gil {

/// \ingroup TIFF_IO
/// \brief Returns the a tiff property.
/// Will throw when file_name is invalid.
template< typename String, typename Property>
bool get_property( const String& file_name, typename Property::type& value, tiff_tag )
{
   return detail::get_property<Property>( file_name, value, tiff_tag() );
}

/// \ingroup TIFF_IO
/// \brief Returns the image info for generating a gil image type.
template< typename String >
basic_tiff_image_read_info read_image_info( const String& file_name, tiff_tag )
{
   basic_tiff_image_read_info info;
   detail::read_image_info( detail::tiff_open_for_read( file_name )
                          , info                                     );
   
   return info;
}

/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
template < typename String, typename Image > 
void read_image( const String& file_name, Image& img, tiff_tag )
{
   detail::tiff_reader<detail::read_and_no_converter> reader( detail::tiff_open_for_read( file_name ));
   reader.apply( img );
}

/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
/// If image is larger a partial image starting from (0,0) is being read.
template< typename String, typename View > 
void read_view( const String&  file_name, const View& v, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Reads the partial image and convert when necessary.
/// Beginning from top_left the image is being read into the view. The dimensions are set by the view.
template< typename String, typename View > 
void read_view( const String& file_name, const View& v, point_t top_left, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Allocates a new image whose dimensions are determined by the given tiff image file, loads and color-converts the pixels into it.
/// Throws std::ios_base::failure if the file is not a valid TIFF file
template <typename String, typename Image>
void read_and_convert_image( const String& file_name, Image& img, tiff_tag )
{
   typedef detail::read_and_convert< default_color_converter > reader_color_convert;

   detail::tiff_reader<reader_color_convert> reader( detail::tiff_open_for_read( file_name ));
   reader.set_color_converter( default_color_converter() );

   reader.apply( img );
}

/// \ingroup TIFF_IO
/// \brief Allocates a new image whose dimensions are determined by the given tiff image file, loads and color-converts the pixels into it.
template< typename String, typename Image, typename Color_Converter >
void read_and_convert_image(const String& file_name, Image& img, Color_Converter cc, tiff_tag )
{
   typedef detail::read_and_convert< Color_Converter > reader_color_convert;

   detail::tiff_reader<reader_color_convert> reader( detail::tiff_open_for_read( file_name ));
   reader.set_color_converter( cc );

   reader.apply( img );
}

/// \ingroup TIFF_IO
/// \brief Loads and color-converts the image specified by the given tiff image file name into the given view.
/// Throws std::ios_base::failure if the file is not a valid TIFF file, or if its dimensions don't match the ones of the view.
template< typename String, typename View >
void read_and_convert_view( const String& file_name, const View& view, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Loads and color-converts the image specified by the given tiff image file name into the given view.
/// Throws std::ios_base::failure if the file is not a valid TIFF file, or if its dimensions don't match the ones of the view.
template< typename String, typename View, typename Color_Converter>
void read_and_convert_view( const String& file_name, const View& view, Color_Converter cc, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Writes the view into an image.
template< typename String, typename View > 
void write_view( const String& file_name, const View& v, tiff_tag )
{
   detail::tiff_writer writer( detail::tiff_open_for_write( file_name ));
   writer.apply( v );
}

/// \ingroup TIFF_IO
/// \brief Writes the view into an image and also add some additional informations.
template< typename String, typename View > 
void write_view( const String& file_name, const View& v, const tiff_image_write_info& write_info, tiff_tag );

} // namespace gil
} // namespace boost

#endif // GIL_TIFF_IO_PRIVATE_HPP
