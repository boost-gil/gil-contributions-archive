#pragma once

#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

namespace boost { namespace gil {

template< typename T
        , typename FormatTag
        , class Enable = void
        >
struct get_write_device
{};


template< typename Device
        , typename FormatTag
        >
struct get_write_device< Device
                       , FormatTag
                       , typename enable_if< mpl::and_< detail::is_adaptable_output_device< FormatTag
                                                                                          , Device
                                                                                          >
                                                      , is_format_tag< FormatTag >
                                                      >
                                           >::type
                       >
{
    typedef typename detail::is_adaptable_output_device< FormatTag
                                                       , Device
                                                       >::device_type type;
};


template< typename String
        , typename FormatTag
        >
struct get_write_device< String
                       , FormatTag
                       , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                      , is_format_tag< FormatTag >
                                                      >
                                           >::type
                       >
{
    typedef detail::file_stream_device< FormatTag > type;
};

} // namespace gil
} // namespace boost
