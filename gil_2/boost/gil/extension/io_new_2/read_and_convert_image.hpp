/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_IMAGE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_IMAGE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief
/// \author Christian Henning and Andreas Pokorny \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/io_new_2/detail/base.hpp>
#include <boost/gil/extension/io_new_2/detail/io_device.hpp>
#include <boost/gil/extension/io_new_2/detail/path_spec.hpp>
#include <boost/gil/extension/io_new_2/detail/conversion_policies.hpp>

#include <boost/gil/extension/io_new_2/detail/jpeg_io_read.hpp>
#include <boost/gil/extension/io_new_2/detail/png_io_read.hpp>
#include <boost/gil/extension/io_new_2/detail/tiff_io_read.hpp>

namespace boost{ namespace gil {

// ------ Image --- Reader ------- With ColorConverter -----------------

/// \ingroup IO
template< typename Device
        , typename Image
        , typename ColorConverter
        , typename FormatTag
        >
inline
void read_and_convert_image( Device&        file
                           , Image&         img
                           , const point_t& top_left
                           , const point_t& bottom_right
                           , const ColorConverter& cc
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                                    , typename detail::is_input_device<Device>::type
                                                                    >::type
                                                >::type* ptr = 0
                           )
{
    typedef detail::read_and_convert< ColorConverter > reader_color_convert;

    detail::reader< Device
                  , FormatTag
                  , reader_color_convert
                  > reader( file
                          , cc
                          );

    reader.read_image( img
                     , top_left
                     , bottom_right
                     );
}

template< typename Device
        , typename Image
        , typename ColorConverter
        , typename FormatTag
        >
inline
void read_and_convert_image( Device&               file
                           , Image&                img
                           , const point_t&        top_left
                           , const point_t&        bottom_right
                           , const ColorConverter& cc
                           , const FormatTag&      tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag<FormatTag>::type
                                                                   , typename detail::is_adaptable_input_device< FormatTag
                                                                                                               , Device
                                                                                                               >::type
                                                                         >::type
                                                     >::type* ptr = 0
                           )
{
    typedef typename detail::is_adaptable_input_device< Device >::device_type device_type;
    device_type dev( file );

    read_and_convert_image( dev
                          , img
                          , top_left
                          , cc
                          , tag
                          );
}

template < typename String
         , typename Image
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_image( const String& file_name
                           , Image& img
                           , const point_t& top_left
                           , const point_t& bottom_right
                           , const ColorConverter& cc
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                                 , typename detail::is_supported_path_spec< String >::type
                                                                 >::type
                                            >::type* ptr = 0
                           )
{
    detail::file_stream_device< FormatTag > device( detail::convert_to_string( file_name )
                                                  , detail::file_stream_device< FormatTag >::read_tag()
                                                  );

    read_and_convert_image( device
                          , img
                          , top_left
                          , bottom_right
                          , cc
                          , tag
                          );
}

/// \ingroup IO
template < typename String
         , typename Image
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_image( const String& file_name
                           , Image& img
                           , const ColorConverter& cc
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                                 , typename detail::is_supported_path_spec< String >::type
                                                                 >::type
                                            >::type* ptr = 0
                           )
{
    read_and_convert_image( file_name
                          , img
                          , point_t( 0, 0 )
                          , point_t( 0, 0 )
                          , cc
                          , tag
                          );
}

template < typename Device
         , typename Image
         , typename ColorConverter
         , typename FormatTag
         >
inline
void read_and_convert_image( Device&               device
                           , Image&                img
                           , const ColorConverter& cc
                           , const FormatTag&      tag
                           , typename enable_if< typename mpl::and_< typename mpl::or_< typename detail::is_input_device< Device >::type
                                                                                                      , typename detail::is_adaptable_input_device< FormatTag
                                                                                                                                                  , Device
                                                                                                                                                  >::type
                                                                                                      >::type
                                                                                   , typename is_format_tag< FormatTag >::type
                                                                                   >::type
                                                               >::type* ptr = 0 
                           )
{
    read_and_convert_image( device
                          , img
                          , point_t( 0, 0 )
                          , point_t( 0, 0 )
                          , cc
                          , tag
                          );
}

/// \ingroup IO
template < typename String
         , typename Image
         , typename FormatTag
         >
inline
void read_and_convert_image( const String&    file_name
                           , Image&           img
                           , const point_t&   top_left
                           , const point_t&   bottom_right
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                                 , typename detail::is_supported_path_spec< String >::type
                                                                 >::type
                                            >::type* ptr = 0
                           )
{
   read_and_convert_image( file_name
                         , img
                         , top_left
                         , bottom_right
                         , default_color_converter()
                         , tag
                         );
}

template < typename Device
         , typename Image
         , typename FormatTag
         >
inline
void read_and_convert_image( Device& device
                           , Image& img
                           , const point_t& top_left
                           , const point_t& bottom_right 
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename mpl::or_< typename detail::is_input_device< Device >::type
                                                                                      , typename detail::is_adaptable_input_device< FormatTag
                                                                                                                                  , Device
                                                                                                                                  >::type
                                                                                      >::type
                                                                   , typename is_format_tag< FormatTag >::type
                                                                   >::type
                                               >::type* ptr = 0
                           )
{
   read_and_convert_image( device
                         , img
                         , top_left
                         , bottom_right
                         , default_color_converter()
                         , tag
                         );
}

/// \ingroup IO
template < typename String
         , typename Image
         , typename FormatTag
         >
inline
void read_and_convert_image( const String&    file_name
                           , Image&           img
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                                 , typename detail::is_supported_path_spec< String >::type
                                                                 >::type
                                            >::type* ptr = 0
                           )
{
   read_and_convert_image( file_name
                         , img
                         , point_t( 0, 0 )
                         , point_t( 0, 0 )
                         , default_color_converter()
                         , tag
                         );
}

template < typename Device
         , typename Image
         , typename FormatTag
         >
inline
void read_and_convert_image( Device&          device
                           , Image&           img
                           , const FormatTag& tag
                           , typename enable_if< typename mpl::and_< typename mpl::or_< typename detail::is_input_device< Device >::type
                                                                                      , typename detail::is_adaptable_input_device< FormatTag
                                                                                                                                  , Device
                                                                                                                                  >::type
                                                                                      >::type
                                                                   , typename is_format_tag< FormatTag >::type
                                                                   >::type
                                               >::type* ptr = 0
                           )
{
   read_and_convert_image( device
                         , img
                         , point_t( 0, 0 )
                         , point_t( 0, 0 )
                         , default_color_converter()
                         , tag
                         );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_READ_AND_CONVERT_IMAGE_HPP_INCLUDED