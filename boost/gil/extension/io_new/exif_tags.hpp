/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_EXIF_TAGS_HPP 
#define BOOST_GIL_EXTENSION_IO_EXIF_TAGS_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include "detail/base.hpp"

namespace boost { namespace gil {

/// Defines exif tag.
struct exif_tag : format_tag {};

/// Read information for bmp images.
///
/// The structure is returned when using read_image_info.
template<>
struct image_read_info< exif_tag >
{
    /// Default contructor.
    image_read_info< exif_tag >()
    : _valid( false )
    {}

    float _ExposureTime;

    float _IsoSpeed;

    float _F_Number;

    float _EvCompensation;

    /// Used internaly to identify is the header has been read.
    bool _valid;
};

/// Read settings for exif data.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings< exif_tag > : public image_read_settings_base
{
    /// Default constructor
    image_read_settings()
    : image_read_settings_base()
    {}

    /// Constructor
    /// \param top_left Top left coordinate for reading partial image.
    /// \param dim      Dimensions for reading partial image.
    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    {}
};

/// Write information for exif data.
///
/// The structure can be used for write_view() function.
template<>
struct image_write_info< exif_tag >
{
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_EXIF_TAGS_HPP
