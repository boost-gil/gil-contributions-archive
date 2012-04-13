#pragma once

#include "get_read_device.hpp"

namespace boost { namespace gil {

///
/// Helper metafunction to generate image reader type.
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
