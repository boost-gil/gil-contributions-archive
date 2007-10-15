/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_JPEG_IO_PRIVATE_HPP
#define GIL_JPEG_IO_PRIVATE_HPP

/// \file
/// \brief 
//
/// \author Christian Henning
/// 

extern "C" {
#include <jpeglib.h>
}

#include <algorithm>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

namespace boost { namespace gil {

struct jpeg_tag {};

} // namespace gil
} // namespace boost


#include <boost/gil/extension/io_new/base.hpp>

namespace boost { namespace gil {

struct basic_jpeg_image_read_info
{
   jpeg_image_width::type  _width;
   jpeg_image_height::type _height;

   jpeg_num_components::type _num_components;
   jpeg_color_space::type    _color_space;
};

struct basic_jpeg_image_write_info
{
   jpeg_quality::type _quality;
};

} // namespace gil
} // namespace boost

#include <boost/gil/extension/io_new/jpeg_io_private.hpp>

namespace boost { namespace gil {

/// \ingroup JPEG_IO
/// \brief Returns the image info for generating a gil image type.
template< typename String >
inline
basic_jpeg_image_read_info read_image_info( const String& file_name, jpeg_tag )
{
   basic_jpeg_image_read_info info;
   detail::read_image_info( detail::jpeg_open_for_read( file_name )
                          , info                                     );

   return info;
}

/// \ingroup JPEG_IO
template < typename String, typename Image > 
inline
void read_image( const String& file_name, Image& img, point_t top_left, jpeg_tag )
{
   detail::jpeg_reader< detail::read_and_no_convert > reader( detail::jpeg_open_for_read( file_name ));
   reader.read_image( img, top_left );
}

/// \ingroup JPEG_IO
template < typename String, typename Image > 
inline
void read_image( const String& file_name, Image& img, const jpeg_tag& tag )
{
   read_image( file_name, img, point_t( 0, 0 ), tag );
}


/// \ingroup JPEG_IO
template< typename String, typename View > 
inline
void read_view( const String& file_name, const View& v, point_t top_left, jpeg_tag )
{
   detail::jpeg_reader< detail::read_and_no_convert > reader( detail::jpeg_open_for_read( file_name ));
   reader.read_view( v, top_left );
}

/// \ingroup JPEG_IO
/// \brief Loads the image specified by the given tiff image file name into the given view.
template< typename String, typename View > 
inline
void read_view( const String&  file_name, const View& v, const jpeg_tag& tag )
{
   read_view( file_name, v, point_t( 0, 0 ), tag );
}

/// \ingroup JPEG_IO
template< typename String, typename Image, typename Color_Converter >
inline
void read_and_convert_image(const String& file_name, Image& img, const point_t& top_left, Color_Converter cc, jpeg_tag )
{
   typedef detail::read_and_convert< Color_Converter > reader_color_convert;

   detail::jpeg_reader< reader_color_convert > reader( detail::jpeg_open_for_read( file_name ));
   reader.set_color_converter( cc );

   reader.read_image( img, top_left );
}

/// \ingroup JPEG_IO
template< typename String, typename Image, typename Color_Converter >
inline
void read_and_convert_image(const String& file_name, Image& img, Color_Converter cc, const jpeg_tag& tag )
{
   read_and_convert_image( file_name, img, point_t( 0, 0 ), cc, tag );
}

/// \ingroup JPEG_IO
template <typename String, typename Image>
inline
void read_and_convert_image( const String& file_name, Image& img, const point_t& top_left, const jpeg_tag& tag )
{
   read_and_convert_image( file_name, img, top_left, default_color_converter(), tag );
}

/// \ingroup JPEG_IO
template <typename String, typename Image>
inline
void read_and_convert_image( const String& file_name, Image& img, const jpeg_tag& tag )
{
   read_and_convert_image( file_name, img, point_t( 0, 0 ), default_color_converter(), tag );
}

/// \ingroup JPEG_IO
template< typename String, typename View, typename Color_Converter>
inline
void read_and_convert_view( const String& file_name, const View& view, const point_t& top_left, Color_Converter cc, jpeg_tag )
{
   typedef detail::read_and_convert< Color_Converter > reader_color_convert;

   detail::jpeg_reader< reader_color_convert > reader( detail::jpeg_open_for_read( file_name ));
   reader.set_color_converter( cc );

   reader.read_view( view, top_left );
}

/// \ingroup JPEG_IO
template< typename String, typename View, typename Color_Converter>
inline
void read_and_convert_view( const String& file_name, const View& view, Color_Converter cc, const jpeg_tag& tag )
{
   read_and_convert_view( file_name, view, point_t( 0, 0 ), cc, tag );
}

/// \ingroup JPEG_IO
template< typename String, typename View >
inline
void read_and_convert_view( const String& file_name, const View& view, const point_t& top_left, const jpeg_tag& tag )
{
   read_and_convert_view( file_name, view, top_left, default_color_converter(), tag );
}

/// \ingroup JPEG_IO
template< typename String, typename View >
inline
void read_and_convert_view( const String& file_name, const View& view, const jpeg_tag& tag )
{
   read_and_convert_view( file_name, view, point_t( 0, 0 ), default_color_converter(), tag );
}

/// \ingroup JPEG_IO
template< typename String, typename View > 
inline
void write_view( const String& file_name, const View& v, const point_t& top_left, jpeg_tag )
{
   detail::jpeg_writer writer( detail::jpeg_open_for_write( file_name ));
   writer.apply( v, top_left );
}

/// \ingroup JPEG_IO
template< typename String, typename View > 
inline
void write_view( const String& file_name, const View& v, const jpeg_tag& tag )
{
   write_view( file_name, v, point_t( 0, 0 ), tag );
}

/// \ingroup JPEG_IO
template< typename String, typename View > 
inline
void write_view( const String& file_name, const View& v, const point_t& top_left, const basic_jpeg_image_write_info& info )
{
   detail::tiff_writer writer( detail::tiff_open_for_write( file_name ));
   writer.apply( v, top_left, info );
}

/// \ingroup JPEG_IO
template< typename String, typename View > 
inline
void write_view( const String& file_name, const View& v, const basic_jpeg_image_write_info& info )
{
   write_view( file_name, v, point_t( 0, 0 ), info );
}

} // namespace gil
} // namespace boost

#endif // GIL_TIFF_IO_PRIVATE_HPP
