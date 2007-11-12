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

   return jpeg_file_t( file, fclose );
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

   return jpeg_file_t( file, fclose );
}

inline
jpeg_file_t jpeg_open_for_write( const std::wstring& file_name )
{
   return jpeg_open_for_write( convert_to_string( file_name ));
}

struct jpeg_decompress_mgr
{
   jpeg_decompress_mgr( jpeg_file_t file )
   {
      _cinfo.err = jpeg_std_error( &_jerr );

      jpeg_create_decompress( &_cinfo             );
      jpeg_stdio_src        ( &_cinfo, file.get() );
      jpeg_read_header      ( &_cinfo, TRUE       );
      
      
      if( jpeg_start_decompress( &_cinfo ) == false )
      {
         io_error( "Cannot start decompression." );
      }

      io_error_if( _cinfo.data_precision != 8, "Image file is not supported." );
   }

   ~jpeg_decompress_mgr()
   {
      jpeg_finish_decompress ( &_cinfo );
      jpeg_destroy_decompress( &_cinfo );
   }

   const jpeg_decompress_struct& get() const
   {
      return _cinfo;
   }

   jpeg_decompress_struct& get()
   {
      return _cinfo;
   }

private:

   jpeg_decompress_struct _cinfo;
   jpeg_error_mgr         _jerr;
};

struct jpeg_compress_mgr
{
   jpeg_compress_mgr( jpeg_file_t file )
   {
      _cinfo.err = jpeg_std_error( &_jerr );

      jpeg_create_compress( &_cinfo             );
      jpeg_stdio_dest     ( &_cinfo, file.get() );
   }

   ~jpeg_compress_mgr()
   {
      jpeg_finish_compress ( &_cinfo );
      jpeg_destroy_compress( &_cinfo );
   }

   jpeg_compress_struct& get()
   {
      return _cinfo;
   }

private:

   jpeg_compress_struct _cinfo;
   jpeg_error_mgr       _jerr;
};


} // namespace details
} // namespace gil
} // namespace boost

#endif // JPEG_BASE_HPP
