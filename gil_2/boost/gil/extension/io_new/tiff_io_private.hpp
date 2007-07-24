/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_IO_PRIVATE_HPP
#define TIFF_IO_PRIVATE_HPP

/// \file
/// \brief  Support for reading and writing TIFF files
//
/// \author Christian Henning
///         

/// see http://www.remotesensing.org/libtiff/
/// or http://www.libtiff.org/man/


#include <fstream>
#include <algorithm>
#include <string>

extern "C" {
#include "tiff.h"
}

namespace boost { namespace gil { 


template <typename Property>
bool get_property( const std::string& file
                 , typename Property::type& value
                 , tiff_tag                        )
{
   TIFF* img = NULL;
   if(( img = TIFFOpen( file.c_str(), "r" )) == NULL )
   {
      throw std::runtime_error( "File doesn't exist." );
   }

   if( TIFFGetFieldDefaulted( img, Property::tag, &value ) == 1 )
   {
      return true;
   }

   TIFFClose( img );

   return false;
}

/*
/todo This specialization wont compile on VS7.1.

template <>
bool get_property<std::string&>( const std::string& file
                               , std::string& value
                               , tiff_tag                 )
{
   if( TIFF* img = TIFFOpen( file.c_str(), "r" ) == NULL )
   {
      throw std::runtime_error( "File doesn't exist." );
   }

   char* buffer = NULL;
   if( TIFFGetFieldDefaulted( img, Property::tag, &buffer ) == 1 )
   {
      return true;
   }

   TIFFClose( img );

   return false;
}
*/

namespace detail {

template< typename String >
void read_image_info( const String& file_name, basic_tiff_image_read_info& info )
{
   get_property<tiff_image_width>( file_name
                                 , info._width
                                 , tiff_tag()   );

   get_property<tiff_image_height>( file_name
                                 , info._height
                                 , tiff_tag()   );

   get_property<tiff_samples_per_pixel>( file_name
                                       , info._samples_per_pixel
                                       , tiff_tag()         );

   get_property<tiff_bits_per_sample>( file_name
                                     , info._bits_per_sample
                                     , tiff_tag()       );

   get_property<tiff_planar_configuration>( file_name
                                          , info._planar_configuration
                                          , tiff_tag()                );
}

} // detail
} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_HPP
