/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_WRITE_VIEW_HPP
#define BOOST_GIL_EXTENSION_IO_WRITE_VIEW_HPP

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

#include "base.hpp"
#include "io_device.hpp"
#include "path_spec.hpp"
#include "conversion_policies.hpp"

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

namespace boost{ namespace gil {

/// \ingroup IO

template< typename Writer
        , typename View
        >
inline
void write_view( Writer&     writer
               , const View& view
               , typename enable_if< typename mpl::and_< typename detail::is_writer< Writer >::type
                                                       , typename is_format_tag< typename Writer::format_tag_t >::type
                                                       , typename is_write_supported< typename get_pixel_type< View >::type
                                                                                    , typename Writer::format_tag_t
                                                                                    >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    writer.apply( view );
}

template< typename Device
        , typename View
        , typename FormatTag
        >
inline
void write_view( Device&          device
               , const View&      view
               , const FormatTag& tag
               , typename enable_if< typename mpl::and_< typename detail::is_write_device< FormatTag
                                                                                         , Device
                                                                                         >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       , typename is_write_supported< typename get_pixel_type< View >::type
                                                                                    , FormatTag
                                                                                    >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_writer( device
                           , tag
                           )
              , view
              );
}

template< typename String
        , typename View
        , typename FormatTag
        >
inline
void write_view( const String&    file_name
               , const View&      view
               , const FormatTag& tag
               , typename enable_if< typename mpl::and_< typename detail::is_supported_path_spec< String >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       , typename is_write_supported< typename get_pixel_type< View >::type
                                                                                    , FormatTag
                                                                                    >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_writer( file_name
                           , tag
                           )
              , view
              );
}

/// \ingroup IO
template< typename Device
        , typename View
        , typename FormatTag
        , typename Log
        >
inline
void write_view( Device&                                 device
               , const View&                             view
               , const image_write_info<FormatTag, Log>& info
               , typename enable_if< typename mpl::and_< typename detail::is_write_device< FormatTag
                                                                                         , Device
                                                                                         >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       , typename is_write_supported< typename get_pixel_type< View >::type
                                                                                    , FormatTag
                                                                                    >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_writer( device
                           , info
                           )
              , view
              );
}

template< typename String
        , typename View
        , typename FormatTag
        , typename Log
        >
inline
void write_view( const String&                             file_name
               , const View&                               view
               , const image_write_info< FormatTag, Log >& info
               , typename enable_if< typename mpl::and_< typename detail::is_supported_path_spec< String >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       , typename is_write_supported< typename get_pixel_type< View >::type
                                                                                    , FormatTag
                                                                                    >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_writer( file_name
                           , info
                           )
              , view
              );
}


////////////////////////////////////// dynamic_image

// without image_write_info
template< typename Writer
        , typename Views
        >
inline
void write_view( Writer&                        writer
               , const any_image_view< Views >& view
               , typename enable_if< typename mpl::and_< typename detail::is_dynamic_image_writer< Writer >::type
                                                       , typename is_format_tag< typename Writer::format_tag_t >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    writer.apply( view );
}

// without image_write_info

template< typename Device
        , typename Views
        , typename FormatTag
        >
inline
void write_view( Device&                        device
               , const any_image_view< Views >& views
               , const FormatTag&
               , typename enable_if< typename mpl::and_< typename detail::is_write_device< FormatTag
                                                                                         , Device
                                                                                         >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_dynamic_image_writer( device
                                         , image_write_info< FormatTag >()
                                         )
              , views
              );
}

template< typename String
        , typename Views
        , typename FormatTag
        >
inline
void write_view( const String&                  file_name
               , const any_image_view< Views >& views
               , const FormatTag&               tag
               , typename enable_if< typename mpl::and_< typename detail::is_supported_path_spec< String >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_dynamic_image_writer( file_name
                                         , image_write_info< FormatTag >()
                                         )
              , views
              );
}

// with image_write_info
/// \ingroup IO
template< typename Device
        , typename Views
        , typename FormatTag
        , typename Log
        >
inline
void write_view( Device&                           device
               , const any_image_view< Views >&    views
               , const image_write_info< FormatTag
                                       , Log
                                       >&           info
               , typename enable_if< typename mpl::and_< typename detail::is_write_device< FormatTag
                                                                                         , Device
                                                                                         >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_dynamic_image_writer( device
                                         , info
                                         )
              , views
              );
}

template< typename String
        , typename Views
        , typename FormatTag
        , typename Log
        >
inline
void write_view( const String&                      file_name
               , const any_image_view< Views >&     views
               , const image_write_info< FormatTag
                                       , Log
                                       >&           info
               , typename enable_if< typename mpl::and_< typename detail::is_supported_path_spec< String >::type
                                                       , typename is_format_tag< FormatTag >::type
                                                       >::type
                                   >::type* /* ptr */ = 0
               )
{
    write_view( make_dynamic_image_writer( file_name
                                         , info
                                         )
              , views
              );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_WRITE_VIEW_HPP
