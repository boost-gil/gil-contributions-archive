/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef JPEG_IO_PRIVATE_HPP
#define JPEG_IO_PRIVATE_HPP

/// \file
/// \brief  Support for reading and writing JPEG files
//
/// \author Christian Henning
///         

/// see http://www.ijg.org/ or http://www.jpeg.org/

#include <fstream>
#include <algorithm>
#include <string>

#include <boost/gil/extension/io_new/jpeg_base.hpp>

namespace boost { namespace gil { namespace detail {

inline
void read_image_info( jpeg_file_t                 file
                    , basic_jpeg_image_read_info& info )
{
   jpeg_decompress_mgr mgr;

   info._width          = mgr.get().image_width;
   info._height         = mgr.get().image_height;
   info._num_components = mgr.get().input_components;
   info._color_space    = mgr.get().in_color_space;
}


struct read_and_no_convert
{
   read_and_no_convert( jpeg_file_t file )
   : _file( file )
   {}

protected:

   jpeg_file_t _file;
};

template< typename Color_Converter >
struct read_and_convert
{
   read_and_convert( jpeg_file_t file )
   : _file( file )
   {}

protected:

   jpeg_file_t _file;
};

template< typename Reader >
class jpeg_reader : public Reader
{
public:

   jpeg_reader( tiff_file_t file )
   : Reader( file )
   {
      read_image_info( file, _info );
   }

   template< typename Image >
   void read_image( Image&         src_img
                  , const point_t& top_left )
   {
      src_img.recreate( _info._width  - top_left.x
                      , _info._height - top_left.y );

      _top_left = top_left;

      apply_impl( view( src_img ));
   }

   template< typename View >
   void read_view( const View&    src_view
                 , const point_t& top_left )
   {
      io_error_if( src_view.dimensions() != point_t( _info._width  - top_left.x
                                                   , _info._height - top_left.y )
                 , "User provided view has incorrect size."                       );
      
      _top_left = top_left;

      apply_impl( src_view );
   }


private:

   template< typename View >
   void apply_impl( const View& src_view )
   {
      jpeg_decompress_mgr mgr;
   }

private:

};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // JPEG_IO_PRIVATE_HPP
