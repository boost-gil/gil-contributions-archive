#pragma once

#include "get_write_device.hpp"

namespace boost { namespace gil {

///
/// Helper metafunction to generate image writer type.
///
template< typename T
        , typename FormatTag
        >
struct get_writer
{
    typedef typename get_write_device< T, FormatTag >::type device_t;

    typedef writer< device_t
                  , FormatTag
                  > type;
};

} // namespace gil
} // namespace boost
