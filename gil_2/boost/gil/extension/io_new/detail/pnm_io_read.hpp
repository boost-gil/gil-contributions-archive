/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/gil/extension/io_new/pnm_tags.hpp>

#include "base.hpp"
#include "reader_base.hpp"
#include "io_device.hpp"
#include "typedefs.hpp"

namespace boost { namespace gil { namespace detail {

template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , pnm_tag
            , ConversionPolicy
            > 
    : public jpeg_decompress_mgr< Device >
    , public reader_base< pnm_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:
    reader( Device& device )
    {}

    reader( Device&     device
          , const cc_t& cc
          )
    : reader_base< pnm_tag
                 , ConversionPolicy >( cc )
    {}

    image_read_info<pnm_tag> get_info()
    {
        image_read_info<pnm_tag> ret;
        return ret;
    }

    template<typename View>
    void apply( const View& view )
    {
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED
