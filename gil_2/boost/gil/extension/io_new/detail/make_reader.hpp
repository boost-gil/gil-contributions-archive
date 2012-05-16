/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_MAKE_READER_HPP
#define BOOST_GIL_EXTENSION_IO_MAKE_READER_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/utility/enable_if.hpp>

#include "get_reader.hpp"

namespace boost { namespace gil {

template< typename String
        , typename FormatTag
        , typename ConversionPolicy
        >
inline
typename get_reader< String
                   , FormatTag
                   , ConversionPolicy
                   >::type
make_reader( const String&    file_name
           , const FormatTag&
           , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                          , is_format_tag< FormatTag >
                                                   >
                               >::type* /* ptr */ = 0
           )
{
    get_read_device< String
                   , FormatTag
                   >::type device( detail::convert_to_native_string( file_name )
                                 , typename detail::file_stream_device< FormatTag >::read_tag()
                                 );

    return get_reader< String
                     , FormatTag
                     , ConversionPolicy
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}

template< typename FormatTag
        , typename ConversionPolicy
        >
inline
typename get_reader< std::wstring
                   , FormatTag
                   >::type
make_reader( const std::wstring& file_name
           , const FormatTag&
           )
{
    const char* str = detail::convert_to_native_string( file_name );

    get_read_device< std::wstring
                   , FormatTag
                   >::type device( str
                                 , typename detail::file_stream_device< FormatTag >::read_tag()
                                 );

    delete[] str;

    return get_reader< std::wstring
                     , FormatTag
                     , ConversionPolicy
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}

#ifdef BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
template< typename FormatTag >
inline
typename get_reader< std::wstring
                   , FormatTag
                   >::type
make_reader( const filesystem::path& path
           , const FormatTag&
           )
{
    return make_reader( path.wstring()
                      , image_read_settings< FormatTag >()
                      );
}
#endif // BOOST_GIL_IO_ADD_FS_PATH_SUPPORT

template< typename Device
        , typename FormatTag
        , typename ConversionPolicy
        >
inline
typename get_reader< Device
                   , FormatTag
                   >::type
make_reader( Device&          file
           , const FormatTag&
           , typename enable_if< mpl::and_< detail::is_adaptable_input_device< FormatTag
                                                                             , Device
                                                                             >
                                          , is_format_tag< FormatTag >
                                          >
                               >::type* /* ptr */ = 0
           )
{
    get_read_device< Device
                   , FormatTag
                   >::type device( file );

    return get_reader< Device
                     , FormatTag
                     , ConversionPolicy
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_MAKE_READER_HPP
