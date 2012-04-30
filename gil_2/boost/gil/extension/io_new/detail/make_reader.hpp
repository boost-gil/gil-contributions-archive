#pragma once

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
typename get_scanline_reader< std::wstring
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


/// Device cannot be const!
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


// scanline_reader

template< typename String
        , typename FormatTag
        >
inline
typename get_scanline_reader< String
                            , FormatTag
                            >::type
make_scanline_reader( const String&    file_name
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

    return get_scanline_reader< String
                              , FormatTag
                              >::type( device
                                     , image_read_settings<FormatTag>()
                                     );
}

template< typename FormatTag >
inline
typename get_scanline_reader< std::wstring
                            , FormatTag
                            >::type
make_scanline_reader( const std::wstring& file_name
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

    return get_scanline_reader< std::wstring
                              , FormatTag
                              >::type( device
                                     , image_read_settings< FormatTag >()
                                     );
}


/// Device cannot be const!
template< typename Device
        , typename FormatTag
        >
inline
typename get_scanline_reader< Device
                            , FormatTag
                            >::type
make_scanline_reader( Device&          file
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

    return get_scanline_reader< Device
                              , FormatTag
                              >::type( device
                                     , image_read_settings< FormatTag >()
                                     );
}


} // namespace gil
} // namespace boost
