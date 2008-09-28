/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_TAGS_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNM_TAGS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include "detail/base.hpp"

namespace boost { namespace gil {

struct pnm_tag : format_tag {};

struct jpeg_image_width
{
   typedef ??? type;
};

struct jpeg_image_height
{
   typedef ??? type;
};

template<>
struct image_read_info< pnm_tag >
{
   pnm_image_width::type  _width;
   pnm_image_height::type _height;
};

template<>
struct image_read_settings< pnm_tag > : public image_read_settings_base
{
    image_read_settings< pnm_tag >()
    : image_read_settings_base()
    {}

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    {}
};


template<>
struct image_write_info< pnm_tag >
{
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_TAGS_HPP_INCLUDED
