/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef JPEG_BASE_HPP
#define JPEG_BASE_HPP

/// \file
/// \brief  Defining some basic libtiff wrappers.
//
/// \author Christian Henning
///         

#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <boost/gil/utilities.hpp>

namespace boost { namespace gil { namespace detail {

typedef boost::shared_ptr<FILE> jpeg_file_t;

inline
jpeg_file_t jpeg_open_for_read( const std::string& file_name )
{
   FILE* file;
   io_error_if( ( file = fopen( file_name.c_str(), "r" )) == NULL
              , "file_mgr: failed to open file" );

   return jpeg_file_t( tiff, fclose );
}

inline
jpeg_file_t jpeg_open_for_read( const std::wstring& file_name )
{
   return jpeg_open_for_read( convert_to_string( file_name ));
}

inline
jpeg_file_t jpeg_open_for_write( const std::string& file_name )
{
   FILE* file;
   io_error_if( ( file = fopen( file_name.c_str(), "w" )) == NULL
              , "file_mgr: failed to open file" );

   return jpeg_file_t( tiff, fclose );
}

inline
jpeg_file_t jpeg_open_for_write( const std::wstring& file_name )
{
   return jpeg_open_for_write( convert_to_string( file_name ));
}

} // namespace details
} // namespace gil
} // namespace boost

#endif // JPEG_BASE_HPP
