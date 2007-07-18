/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_TIFF_IO_HPP
#define GIL_TIFF_IO_HPP

/// \file
/// \brief  Support for reading and writing TIFF files
//
/// \author Christian Henning
///         

#include <cstdio>
#include <algorithm>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/gil/extension/io/io_base.hpp>

namespace boost { namespace gil {

struct tiff_tag {};

/// \ingroup TIFF_IO
/// This struct has all the information that gil needs to create the appropriate image type.
/// It should be a little different from file format to file format.
struct basic_tiff_image_read_info
{
   std::size_t _width;
   std::size_t _height;
   std::size_t _bits_per_sample;
   std::size_t _samples_per_pixel;
};

/// \ingroup TIFF_IO
/// Ths file-format specific struct should contain all information the file-format offers. Obviously
/// it will vary quite a bit from format to format.
struct tiff_image_read_info
{
   // Baseline TIFF tags as listed in the core TIFF spec.
   // See: http://www.awaresystems.be/imaging/tiff/tifftags/baseline.html

   unsigned long  _NewSubfileType;
   unsigned short _SubfileType;
   
   unsigned short _Compression;

   unsigned short _PhotometricInterpretation;

   unsigned short _Threshholding;
   
   unsigned short _CellWidth;
   unsigned short _CellLength;

   unsigned short _FillOrder;

   std::string _ImageDescription;

   std::string _Make;
   std::string _Model;

   unsigned long* _StripOffsets;

   unsigned short _Orientation;

   unsigned long _RowsPerStrip;
   unsigned long* _StripByteCounts;

   unsigned short* _MinSampleValue;
   unsigned short* _MaxSampleValue;

   float _XResolution;
   float _YResolution;

   unsigned short _PlanarConfiguration;

   unsigned long* _FreeOffsets;
   unsigned long* _FreeByteCounts;

   unsigned short _GrayResponseUnit;
   unsigned short* _GrayResponseCurve;
   
   unsigned short _ResolutionUnit;

   std::string _Software;
   std::string _DateTime;
   std::string _Artist;
   std::string _HostComputer;

   unsigned short* ColorMap;

   unsigned short* ExtraSamples;

   std::string _Copyright;
};

/// \ingroup TIFF_IO
/// There is no difference between the read and write info struct.
struct tiff_image_write_info : public tiff_image_read_info {};


/// \ingroup TIFF_IO
/// \brief Returns the image info for generating a gil image type.
void read_image_info( const std::string&  file_name, basic_tiff_image_read_info info, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Returns the image info for generating a gil image type.
void read_image_info( const std::wstring& file_name, basic_tiff_image_read_info info, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Retrieves all information from the image file header.
void read_detailed_image_info( const std::string&  file_name, tiff_image_read_info& info, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Retrieves all information from the image file header.
void read_detailed_image_info( const std::wstring& file_name, tiff_image_read_info& info, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
template < typename Image > void read_image( const std::string&  file_name, Image& img, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
template < typename Image > void read_image( const std::wstring& file_name, Image& img, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
/// If image is larger a partial image starting from (0,0) is being read.
template< typename View > void read_view( const std::string&  file_name, const View& v, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Reads the image and convert when necessary.
/// If image is larger a partial image starting from (0,0) is being read.
template< typename View > void read_view( const std::wstring& file_name, const View& v, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Reads the partial image and convert when necessary.
/// Beginning from top_left the image is being read into the view. The dimensions are set by the view.
template< typename View > void read_view( const std::string& file_name, const View& v, point_t top_left, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Reads the partial image and convert when necessary.
/// Beginning from top_left the image is being read into the view. The dimensions are set by the view.
template< typename View > void read_view( const std::wstring& file_name, const View& v, point_t top_left, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Writes the view into an image.
template< typename View > void write_view( const std::string&  file_name, const View& v, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Writes the view into an image.
template< typename View > void write_view( const std::wstring& file_name, const View& v, tiff_tag );


/// \ingroup TIFF_IO
/// \brief Writes the view into an image and also add some additional informations.
template< typename View > void write_view( const std::string&  file_name, const View& v, const tiff_image_write_info& write_info, tiff_tag );

/// \ingroup TIFF_IO
/// \brief Writes the view into an image and also add some additional informations.
template< typename View > void write_view( const std::wstring& file_name, const View& v, const tiff_image_write_info& write_info, tiff_tag );

} // namespace gil
} // namespace boost

#endif // GIL_TIFF_IO_HPP
