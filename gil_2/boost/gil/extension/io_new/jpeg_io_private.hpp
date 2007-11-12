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
#include <boost/gil/extension/io_new/jpeg_supported_read_types.hpp>

namespace boost { namespace gil { namespace detail {

inline
void read_image_info_impl( const jpeg_decompress_mgr   mgr
                         , basic_jpeg_image_read_info& info )
{
   info._width          = mgr.get().image_width;
   info._height         = mgr.get().image_height;
   info._num_components = mgr.get().num_components;
   info._color_space    = mgr.get().out_color_space;
}

inline
void read_image_info( jpeg_file_t                 file
                    , basic_jpeg_image_read_info& info )
{
   jpeg_decompress_mgr mgr( file );

   read_image_info_impl( mgr, info );
}

struct read_and_no_convert
{
   read_and_no_convert( jpeg_file_t file )
   : _file( file )
   {}

protected:

   template< typename Image_JPEG
           , typename View_User
           >
   void read( const View_User&                  src_view
            , const point_t&                    top_left
            , const basic_jpeg_image_read_info& info
            , boost::mpl::false_                          )
   {
      typedef typename Image_JPEG::view_t view_jpeg_t;
      typedef typename view_jpeg_t::value_type pixel_t;

      io_error_if( views_are_compatible<View_User, view_jpeg_t>::value != true
                 , "Views are incompatible. You might want to use read_and_convert_image()" );

      typedef std::vector< view_jpeg_t::value_type > buffer_t;
      buffer_t buffer( src_view.width() );

      JSAMPLE* row_address = reinterpret_cast<JSAMPLE*>( &buffer.front() );

      jpeg_decompress_mgr mgr( _file );

      for( int y = 0; y < src_view.height(); ++y )
      {
         io_error_if( jpeg_read_scanlines( &mgr.get()
                                         , (JSAMPARRAY) &row_address
                                         , 1 ) != 1
                    , "jpeg_reader::apply(): fail to read JPEG file" );

         //std::copy( buffer.begin(), buffer.end(), src_view.row_begin( y ));
      }
   }

   template< typename Image_JPEG
           , typename View_User
           >
   void read( const View_User&                  src_view
            , const point_t&                    top_left
            , const basic_jpeg_image_read_info& info
            , boost::mpl::true_                            )
   {
      io_error( "Image type not supported." );
   }

protected:

   jpeg_file_t _file;
};

template< typename ColorConverter >
struct read_and_convert
{
   read_and_convert( jpeg_file_t file )
   : _file( file )
   {}

   void set_color_converter( const ColorConverter& cc )
   {
      _cc = cc;
   }

protected:

   template< typename Image_JPEG
           , typename View_User
           >
   void read( const View_User&                  src_view
            , const point_t&                    top_left
            , const basic_jpeg_image_read_info& info
            , const jpeg_decompress_struct&     decompress_mgr
            , boost::mpl::false_                            )
   {
      std::vector<gray8_pixel_t> row(view.width());
      JSAMPLE* row_address=(JSAMPLE*)&row.front();

      jpeg_decompress_mgr mgr( _file );

      for(int y=0;y<view.height();++y)
      {
         io_error_if( jpeg_read_scanlines( &mgr.get(), (JSAMPARRAY) &row_address, 1 ) !=1
                    , "jpeg_reader_color_covert::apply(): fail to read JPEG file"      );

         std::transform( row.begin()
                       , row.end()
                       ,view.row_begin( y )
                       ,color_convert_deref_fn< gray8_ref_t
                                              , typename View::value_type,CC
                                              >( _cc ));
      }
   }

   template< typename Image_JPEG
           , typename View_User
           >
   void read( const View_User&                  src_view
            , const point_t&                    top_left
            , const basic_jpeg_image_read_info& info
            , boost::mpl::true_                            )
   {
      io_error( "Image type not supported." );
   }

protected:

   jpeg_file_t _file;

   ColorConverter _cc;
};


template< typename Reader >
class jpeg_reader : public Reader
{
public:

   jpeg_reader( jpeg_file_t file )
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
                 , "User provided view has incorrect size."                                );
      
      _top_left = top_left;

      apply_impl( src_view, info );
   }


private:

   template< typename View >
   void apply_impl( const View& src_view )
   {
      jpeg_decompress_mgr mgr( _file );

      switch( _info._color_space )
      {
         case JCS_GRAYSCALE:
         {
            //num_components< gray_layout_t >( src_view );
            break;
         }
         
         case JCS_RGB:
         {
            num_components< rgb_layout_t >( src_view );
            break;
         }

         case JCS_CMYK:
         {
            //num_components< cmyk_layout_t >( src_view );
            break;
         }

         default:
         {
            io_error( "Image file is not supported." );
         }
      }
   }

   template< typename Layout
           , typename View
           >
   void num_components( const View& src_view )
   {
      switch( _info._num_components )
      {
         case 1:
         {
            //read_image< 1, Layout >( src_view );
            break;
         }

         case 3:
         {
            read_image< 3, Layout >( src_view );
            break;
         }

         case 4:
         {
            //read_image< 4, Layout >( src_view );
            break;
         }

         default:
         {
            io_error( "Image file is not supported." );
            break;
         }
      }
   }

   template< int      NumComponents
           , typename Layout
           , typename View
           >
   void read_image( const View& src_view )
   {
      typedef image_type_factory< NumComponents, Layout >::type image_t;

      typedef is_same< image_t, not_allowed_t >::type unspecified_t;

      read< image_t >( src_view
                     , _top_left
                     , _info
                     , unspecified_t() );
   }


private:

   point_t _top_left;

   basic_jpeg_image_read_info _info;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // JPEG_IO_PRIVATE_HPP
