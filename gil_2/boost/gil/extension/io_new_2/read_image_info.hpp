/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_IMAGE_READ_INFO_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_IMAGE_READ_INFO_HPP_INCLUDED

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

/// \ingroup IO
/// \brief Returns the image info for generating a gil image type.
template< typename Device
        , typename FormatTag
        >
inline
image_read_info< FormatTag >
read_image_info( Device&         file
               , const FormatTag tag
               , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                       , typename detail::is_input_device<Device>::type
                                                       >::type
                                                >::type* ptr = 0
               )
{
    return detail::reader< Device
                         , FormatTag
                         , detail::read_and_no_convert
                         >( file ).get_info();
}

template< typename Device
        , typename FormatTag
        >
inline 
image_read_info<FormatTag>
read_image_info( Device&          file
               , const FormatTag& tag
               , typename enable_if< typename mpl::and_< typename is_format_tag<FormatTag>::type
                                                       , typename detail::is_adaptable_input_device< FormatTag
                                                                                                   , Device
                                                                                                   >::type
                                                                         >::type
                                   >::type* ptr = 0
               )
{
    typedef typename detail::is_adaptable_input_device< FormatTag
                                                      , Device
                                                      >::device_type device_type;

    device_type dev( file );
    return detail::reader< device_type
                         , FormatTag
                         , detail::read_and_no_convert
                         >( file ).get_info();
}

template< typename String
        , typename FormatTag
        >
inline 
image_read_info<FormatTag>  
read_image_info( const String&    file_name
               , const FormatTag& tag
               , typename enable_if< typename mpl::and_< typename is_format_tag< FormatTag >::type
                                                       , typename detail::is_supported_path_spec< String >::type
                                                       >::type
                                   >::type* ptr = 0
               )
{
    detail::file_stream_device< FormatTag > reader( detail::convert_to_string( file_name )
                                                  , detail::file_stream_device< FormatTag >::read_tag()
                                                  );

    return read_image_info( reader
                          , tag    );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_IMAGE_READ_INFO_HPP_INCLUDED