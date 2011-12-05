#pragma once

#include <boost/utility/enable_if.hpp>

#include "get_writer.hpp"

namespace boost { namespace gil {

template< typename String
        , typename FormatTag
        >
inline
typename get_writer< String
                   , FormatTag
                   >::type
make_writer( const String&    file_name
             , const FormatTag&
             , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                            , is_format_tag< FormatTag >
                                            >
                                 >::type* /* ptr */ = 0
             )
{
    get_write_device< String
                    , FormatTag
                    >::type device( detail::convert_to_native_string( file_name )
                                  , typename detail::file_stream_device< FormatTag >::write_tag()
                                  );

    return get_writer< String
                     , FormatTag
                     >::type( device );
}

template< typename FormatTag >
inline
typename get_writer< std::wstring
                   , FormatTag
                   >::type
make_writer( const std::wstring& file_name
             , const FormatTag&
             )
{
    get_write_device< std::wstring
                    , FormatTag
                    >::type device( detail::convert_to_native_string( file_name )
                                  , typename detail::file_stream_device< FormatTag >::write_tag()
                                  );

    return get_writer< std::wstring
                     , FormatTag
                     >::type( device );
}

template< typename Device
        , typename FormatTag
        >
inline
typename get_writer< Device
                   , FormatTag
                   >::type
make_writer( Device& file
             , const FormatTag&
             , typename enable_if< mpl::and_< typename detail::is_adaptable_output_device< FormatTag, Device >::type
                                            , is_format_tag< FormatTag >
                                            >
                                 >::type* /* ptr */ = 0
             )
{
    typename detail::is_adaptable_output_device< FormatTag
                                               , Device
                                               >::device_type device( file );

    return get_writer< Device
                     , FormatTag
                     >::type( device );
}

} // namespace gil
} // namespace boost
