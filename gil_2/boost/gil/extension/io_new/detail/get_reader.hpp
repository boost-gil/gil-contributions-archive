#pragma once

#include "get_read_device.hpp"

namespace boost { namespace gil {

///
/// Helper metafunction to generate image reader type.
///
template< typename T
        , typename FormatTag
        , typename ConversionPolicy = detail::read_and_no_convert
        >
struct get_reader
{
    typedef typename get_read_device< T
                                    , FormatTag
                                    >::type device_t;

    typedef reader< device_t
                  , FormatTag
                  , ConversionPolicy
                  > type;
};

///
/// Helper metafunction to generate image backend type.
///

template< typename T
        , typename FormatTag
        , class Enable = void
        >
struct get_reader_backend
{};

template< typename String
        , typename FormatTag
        >
struct get_reader_backend< String
                         , FormatTag
                         , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type
                         >
{
    typedef typename get_read_device< String
                                    , FormatTag
                                    >::type device_t;

    typedef reader_backend< device_t
                          , FormatTag
                          > type;
};

template< typename Device
        , typename FormatTag
        >
struct get_reader_backend< Device
                         , FormatTag
                         , typename enable_if< mpl::and_< detail::is_adaptable_input_device< FormatTag
                                                                                           , Device
                                                                                           >
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type
                         >
{
    typedef typename get_read_device< Device
                                    , FormatTag
                                    >::type device_t;

    typedef reader_backend< device_t
                          , FormatTag
                          > type;
};

///
/// Helper metafunction to generate image scanline_reader type.
///
template< typename T
        , typename FormatTag
        >
struct get_scanline_reader
{
    typedef typename get_read_device< T
                                    , FormatTag
                                    >::type device_t;

    typedef scanline_reader< device_t
                           , FormatTag
                           > type;
};

} // namespace gil
} // namespace boost
