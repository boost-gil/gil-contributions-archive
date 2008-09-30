/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/gil/extension/io_new/bmp_tags.hpp>

namespace boost { namespace gil { namespace detail {

template< typename Device >
class writer< Device
            , bmp_tag
            > 
{
public:

    writer( Device & file )
        : out(file)
    {
    }

    ~writer()
    {
    }

    template<typename View>
    void apply( const View& view )
    {
    }

    template<typename View>
    void apply( const View&                       view
              , const image_write_info<jpeg_tag>& info )
    {
    }

private:

    Device& _out;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED
