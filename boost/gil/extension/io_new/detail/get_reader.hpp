#pragma once

#include "get_read_device.hpp"

namespace boost { namespace gil {

///
/// Helper metafunction to generate image reader type.
///
template< typename T
        , typename FormatTag
        , typename ConversionPolicy
        , typename View
        >
struct get_reader
{
    typedef typename get_read_device< T
                                    , FormatTag
                                    >::type device_t;

    typedef reader< device_t
                  , FormatTag
                  , ConversionPolicy
                  , View
                  > type;
};

} // namespace gil
} // namespace boost
