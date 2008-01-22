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

#include <boost/gil/extension/io_new_2/tiff_tags.hpp>

#include <boost/gil/extension/io_new_2/detail/base.hpp>
#include <boost/gil/extension/io_new_2/detail/tiff_io_private.hpp>

namespace boost { namespace gil { namespace detail {

template< typename Device
        , typename ConversionPolicy
        >
struct reader< Device
             , tiff_tag
             , ConversionPolicy > 
{
public:

   reader( Device& device )
   : _io_dev( device )
   {}

   reader( Device&                                                device
         , typename const ConversionPolicy::color_converter_type& cc     )
   : _io_dev   ( device )
   , _cc_policy( cc     )
   {
   }

   image_read_info<tiff_tag> get_info()
   {
      image_read_info<tiff_tag> info = {0};

      io_error_if( _io_dev.get_property<tiff_image_width>               ( info._width ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_image_height>              ( info._height ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_compression>               ( info._compression ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_samples_per_pixel>         ( info._samples_per_pixel ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_bits_per_sample>           ( info._bits_per_sample ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_sample_format>             ( info._sample_format ) == false
                 , "cannot read tiff tag." );
      io_error_if( _io_dev.get_property<tiff_planar_configuration>      ( info._planar_configuration ) == false
                 , "cannot read tiff tag." );

      ///@todo: Doesn't work right now.
      //io_error_if( _io_dev.get_property<tiff_photometric_interpretation>( info._photometric_interpretation  ) == false
      //           , "cannot read tiff tag." );

      return info;
   }

    template< typename Image >
    void read_image( Image&         image
                   , const point_t& top_left )
    {
        image_read_info<tiff_tag> info( get_info() );
        image.recreate( info._width  - top_left.x
                      , info._height - top_left.y );

        apply_impl( view( image )
                  , top_left
                  , info          );
    }

    template<typename View>
    void read_view( View&          view
                  , const point_t& top_left )
    {
        image_read_info<tiff_tag> info( get_info() );
        io_error_if( view.dimensions() !=  point_t( info._width  - top_left.x
                                                  , info._height - top_left.y )
                , "User provided view has incorrect size."                       );

        apply_impl( view
                  , top_left
                  , info      );
    }
private:

    template< typename View >
    void apply_impl( View&                            view
                   , const point_t&                   top_left
                   , const image_read_info<tiff_tag>& info     )
    {
    }

   template< typename ImagePixel
         , typename View
         >
   void read_rows( View&          view
               , const point_t& top_left )
   {
      io_error_if( ! ConversionPolicy::template is_allowed<ImagePixel,typename View::value_type>::type::value,
               "User provided view has incorrect color space or channel type.");

      std::vector<ImagePixel> buffer( view.width() );

      for( int y = 0; y < view.height(); ++y )
      {
         cc_policy.read( buffer.begin() + top_left.x
                        , buffer.end()
                        , view.row_begin( y )          );
      }
   }

    Device& _io_dev;

    ConversionPolicy _cc_policy;
};

template < typename Device >
struct writer< Device
             , tiff_tag
             > 
{
   writer( Device& file )
   : _out(file)
   {
   }

   ~writer()
   {
   }

   template<typename View>
   void apply( const View&    view
             , const point_t& top_left
             , boost::mpl::true_       )
   {
      write_rows( view
                , top_left );
   }

    template<typename View>
    void apply( const View& view
              , const point_t& top_left
              , boost::mpl::false_      )
    {
        write_rows(view, top_left);
    }

    template<typename View>
    void apply( const View&                       view
              , const point_t&                    top_left
              , const image_write_info<tiff_tag>& info      )
    {
        write_rows( view
                  , top_left );
    }

private:

    template< typename View >
    void write_rows( const View&    view
                   , const point_t& top_left )
    {
    }

    Device& _out;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // GIL_TIFF_IO_PRIVATE_HPP
