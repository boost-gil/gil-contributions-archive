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

#include <algorithm>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <boost/gil/extension/io_new/base.hpp>

namespace boost { namespace gil {

struct tiff_tag {};

typedef boost::fusion::vector2< bool, boost::uint32_t > ulong_tag_t;
typedef boost::fusion::vector2< bool, boost::uint16_t > ushort_tag_t;
typedef boost::fusion::vector2< bool, float >           float_tag_t;
typedef boost::fusion::vector2< bool, std::string >     string_tag_t;
typedef boost::fusion::vector2< bool, std::vector< unsigned long  > > ulong_vec_tag_t;
typedef boost::fusion::vector2< bool, std::vector< unsigned short > > ushort_vec_tag_t;


/// \ingroup TIFF_IO
/// This struct has all the information that gil needs to create the appropriate image type.
/// It should be a little different from file format to file format.
struct basic_tiff_image_read_info
{
   ulong_tag_t  _width;
   ulong_tag_t  _height;
   ushort_tag_t _samples_per_pixel;

   // Apparently, this item is not an array of ushorts. 
   // see http://www.libtiff.org/man/TIFFGetField.3t.html
   ushort_tag_t _bits_per_sample;
};

/// \ingroup TIFF_IO
/// Ths file-format specific struct should contain all information the file-format offers. Obviously
/// it will vary quite a bit from format to format.
struct tiff_image_read_info
{
   // Baseline TIFF tags as listed in the core TIFF spec.
   // See: http://www.awaresystems.be/imaging/tiff/tifftags/baseline.html

   ulong_tag_t  _NewSubfileType;
   ushort_tag_t _SubfileType;
   
   ushort_tag_t _Compression;

   ushort_tag_t _PhotometricInterpretation;

   ushort_tag_t _Threshholding;

   ushort_tag_t _CellWidth;
   ushort_tag_t _CellLength;

   ushort_tag_t _FillOrder;

   string_tag_t _ImageDescription;

   string_tag_t _Make;
   string_tag_t _Model;

   ulong_vec_tag_t _StripOffsets;

   ushort_tag_t _Orientation;

   ulong_tag_t     _RowsPerStrip;
   ulong_vec_tag_t _StripByteCounts;

   ushort_vec_tag_t _MinSampleValue;
   ushort_vec_tag_t _MaxSampleValue;

   float_tag_t _XResolution;
   float_tag_t _YResolution;

   ushort_tag_t _PlanarConfiguration;

   ulong_vec_tag_t _FreeOffsets;
   ulong_vec_tag_t _FreeByteCounts;

   ushort_tag_t     _GrayResponseUnit;
   ushort_vec_tag_t _GrayResponseCurve;
   
   ushort_tag_t _ResolutionUnit;

   string_tag_t _Software;
   string_tag_t _DateTime;
   string_tag_t _Artist;
   string_tag_t _HostComputer;

   ushort_vec_tag_t _ColorMap;

   ushort_vec_tag_t _ExtraSamples;

   string_tag_t _Copyright;

   static const unsigned int _version = 0;
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

#endif // GIL_TIFF_IO_PRIVATE_HPP
