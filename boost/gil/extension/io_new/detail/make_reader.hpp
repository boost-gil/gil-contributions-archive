#pragma once

#include <boost/utility/enable_if.hpp>

#include "get_reader.hpp"

namespace boost { namespace gil {

template< typename String
        , typename FormatTag
        , typename ConversionPolicy
        , typename View
        >
inline
typename get_reader< String
                   , FormatTag
                   , ConversionPolicy
                   , View
                   >::type
make_reader( const String&    file_name
           , const FormatTag&
           , const ConversionPolicy&
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
                     , View
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}

template< typename FormatTag
        , typename ConversionPolicy
        , typename View
        >
inline
typename get_reader< std::wstring
                   , FormatTag
                   , View
                   , ConversionPolicy
                   >::type
make_reader( const std::wstring& file_name
           , const FormatTag&
           , const ConversionPolicy&
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
                     , View
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}


/// Device cannot be const!
template< typename Device
        , typename FormatTag
        , typename ConversionPolicy
        , typename View
        >
inline
typename get_reader< Device
                   , FormatTag
                   , View
                   , ConversionPolicy
                   >::type
make_reader( Device&    file
           , const FormatTag&
           , const ConversionPolicy&
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
                     , View
                     >::type( device
                            , image_read_settings< FormatTag >()
                            );
}

} // namespace gil
} // namespace boost
