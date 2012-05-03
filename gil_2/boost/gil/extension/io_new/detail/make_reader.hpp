#pragma once

#include <boost/utility/enable_if.hpp>

#include "get_reader.hpp"

namespace boost { namespace gil {

////////////////////////////////////////////////////////////
// reader
////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////
// backend
////////////////////////////////////////////////////////////

template< typename String
        , typename FormatTag
        >
inline
typename get_reader_backend< String
                           , FormatTag
                           >::type
make_reader_backend( const String&                           file_name
                   , const image_read_settings< FormatTag >& settings
                   , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                  , is_format_tag< FormatTag >
                                                           >
                                       >::type* /* ptr */ = 0
                   )
{
    typedef get_read_device< String
                           , FormatTag
                           >::type device_t;

    device_t device( detail::convert_to_native_string( file_name )
                   , typename detail::file_stream_device< FormatTag >::read_tag()
                   );

    return reader_backend< device_t, FormatTag >( device, settings );
}

template< typename FormatTag >
inline
typename get_scanline_reader< std::wstring
                            , FormatTag
                            >::type
make_reader_backend( const std::wstring&                     file_name
                   , const image_read_settings< FormatTag >& settings
                   )
{
    typedef get_read_device< std::wstring
                           , FormatTag
                           >::type device_t;
                   
    const char* str = detail::convert_to_native_string( file_name );

    device_t device( str
                   , typename detail::file_stream_device< FormatTag >::read_tag()
                   );

    delete[] str;

    return reader_backend< device_t, FormatTag >( device, settings );
}

template< typename Device
        , typename FormatTag
        >
inline
typename get_reader_backend< Device
                           , FormatTag
                           >::type
make_reader_backend( Device&                                 io_dev
                   , const image_read_settings< FormatTag >& settings
                   , typename enable_if< mpl::and_< detail::is_adaptable_input_device< FormatTag
                                                                                     , Device
                                                                                     >
                                                  , is_format_tag< FormatTag >
                                                  >
                                       >::type* /* ptr */ = 0
                   )
{
    typedef get_read_device< Device
                           , FormatTag
                           >::type device_t;

    device_t device( io_dev );

    return reader_backend< device_t, FormatTag >( device, settings );
}



template< typename String
        , typename FormatTag
        >
inline
typename get_reader_backend< String
                           , FormatTag
                           >::type
make_reader_backend( const String&    file_name
                   , const FormatTag&
                   , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                  , is_format_tag< FormatTag >
                                                           >
                                       >::type* /* ptr */ = 0
                   )
{
    return make_reader_backend( file_name, image_read_settings< FormatTag >() );
}

template< typename Device
        , typename FormatTag
        >
inline
typename get_reader_backend< Device
                           , FormatTag
                           >::type
make_reader_backend( Device&          io_dev
                   , const FormatTag& tag
                   , typename enable_if< mpl::and_< detail::is_adaptable_input_device< FormatTag
                                                                                     , Device
                                                                                     >
                                                  , is_format_tag< FormatTag >
                                                  >
                                       >::type* /* ptr */ = 0
                   )
{
    return make_reader_backend( io_dev, image_read_settings< FormatTag >() );
}

////////////////////////////////////////////////////////////
// scanline_reader
////////////////////////////////////////////////////////////

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

//template< typename FormatTag >
//inline
//typename get_scanline_reader< std::wstring
//                            , FormatTag
//                            >::type
//make_scanline_reader( const std::wstring& file_name
//                    , const FormatTag&
//                    )
//{
//    const char* str = detail::convert_to_native_string( file_name );
//
//    get_read_device< std::wstring
//                   , FormatTag
//                   >::type device( str
//                                 , typename detail::file_stream_device< FormatTag >::read_tag()
//                                 );
//
//    delete[] str;
//
//    return get_scanline_reader< std::wstring
//                              , FormatTag
//                              >::type( device
//                                     , image_read_settings< FormatTag >()
//                                     );
//}


template< typename Device
        , typename FormatTag
        >
inline
typename get_scanline_reader< Device
                            , FormatTag
                            >::type
make_scanline_reader( Device&          io_dev
                    , const FormatTag&
                    , typename enable_if< mpl::and_< detail::is_adaptable_input_device< FormatTag
                                                                                      , Device
                                                                                      >
                                                   , is_format_tag< FormatTag >
                                                   >
                                        >::type* /* ptr */ = 0
                    )
{
    return make_scanline_reader( io_dev, image_read_settings< FormatTag >() );
}


} // namespace gil
} // namespace boost
