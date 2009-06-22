/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP 

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include <png.h>
}

#include <string>
#include <vector>

#include "detail/base.hpp"

namespace boost { namespace gil {

struct png_tag : format_tag {};

struct png_image_width  : property_base< png_uint_32 > {};
struct png_image_height : property_base< png_uint_32 > {};

struct png_interlace_method   : property_base< int > {};
struct png_compression_method : property_base< int > {};
struct png_filter_method      : property_base< int > {};
struct png_bitdepth           : property_base< int > {};
struct png_color_type         : property_base< int > {};

struct png_num_channels       : property_base< png_byte > {};

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
    struct png_chromacities_type : property_base< double > {};

    struct png_gamma : property_base< double > {};

    struct png_unit  : property_base< int    > {};
    struct png_scale : property_base< double > {};

#else 
    struct png_chromacities_type : property_base< png_fixed_point > {};

    struct png_gamma : property_base< png_fixed_point > {};

    struct png_unit  : property_base< int         > {};
    struct png_scale : property_base< std::string > {};

#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

struct png_ICC_name             : property_base< std::string > {};
struct png_ICC_profile          : property_base< std::string > {};
struct png_ICC_profile_length   : property_base< png_uint_32 > {};
struct png_ICC_compression_type : property_base< int         > {};

struct png_intent : property_base< int > {};

struct png_color_palette : property_base< std::vector< png_color > > {};
struct png_num_palette   : property_base< int > {};

struct png_background : property_base< png_color_16p > {};

struct png_histrogram : property_base< png_uint_16p > {};

struct png_offset      : property_base< png_int_32 > {};
struct png_offset_type : property_base< int > {};

struct png_CAL        : property_base< std::string > {};
struct png_CAL_params : property_base< std::vector< std::string > > {};
struct png_CAL_X      : property_base< png_int_32 > {};
struct png_CAL_type   : property_base< int > {};
struct png_CAL_nparam : property_base< int > {};

struct png_resolution : property_base< png_uint_32 > {};
struct png_unit_type  : property_base< int > {};

struct png_significant_bits : property_base< png_color_8p > {};

struct gil_io_png_text
{
    int         _compression;
    std::string _key;
    std::string _text;
};

struct png_text     : property_base< std::vector< gil_io_png_text > > {};
struct png_num_text : property_base< int > {};

struct png_mod_time : property_base< png_timep > {};

struct png_trans        : property_base< std::vector< png_byte > > {};
struct png_num_trans    : property_base< int > {};
struct png_trans_values : property_base< std::vector< png_color_16 > > {};

template<>
struct image_read_info< png_tag >
{
    png_image_width::type  _width;
    png_image_height::type _height;

    png_bitdepth::type           _bit_depth;
    png_color_type::type         _color_type;
    png_interlace_method::type   _interlace_method;
    png_compression_method::type _compression_method;
    png_filter_method::type      _filter_method;

    png_num_channels::type _num_channels;

    // CIE chromacities
    png_chromacities_type::type _white_x;
    png_chromacities_type::type _white_y;
    png_chromacities_type::type _red_x;
    png_chromacities_type::type _red_y;
    png_chromacities_type::type _green_x;
    png_chromacities_type::type _green_y;
    png_chromacities_type::type _blue_x;
    png_chromacities_type::type _blue_y;

    // Gamma Value
    png_gamma::type _gamma;

    // Embedded ICC profile
    png_ICC_name::type             _icc_name;
    png_ICC_compression_type::type _iccp_compression_type;
    png_ICC_profile::type          _profile;
    png_ICC_profile_length::type   _profile_length;

    // Rendering intent
    png_intent::type _intent;

    // Image palette
    png_color_palette::type _palette;
    png_num_palette::type   _num_palette;

    // Background
    png_background::type _background;

    // Histogram
    png_histrogram::type _histogram;

    // Screen offsets
    png_offset::type      _offset_x;
    png_offset::type      _offset_y;
    png_offset_type::type _off_unit_type;

    // Pixel Calibration
    png_CAL::type        _purpose;
    png_CAL_X::type      _X0;
    png_CAL_X::type      _X1;
    png_CAL_type::type   _cal_type;
    png_CAL_nparam::type _num_params;
    png_CAL::type        _units;
    png_CAL_params::type _params;

    // Physical resolution
    png_resolution::type _res_x;
    png_resolution::type _res_y;
    png_unit_type::type  _phy_unit_type;

    // Number of significant bits
    png_significant_bits::type _sig_bits;

    // Scale Factors
    png_unit::type  _scale_unit;
    png_scale::type _scale_width;
    png_scale::type _scale_height;

    // Comments information
    png_text::type     _text;
    png_num_text::type _num_text;

    // Last modification time
    png_mod_time::type _mod_time;

    // Transparency data
    png_trans::type        _trans;
    png_num_trans::type    _num_trans;
    png_trans_values::type _trans_values;
};

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

template<>
struct image_read_settings< png_tag > : public image_read_settings_base
{
    image_read_settings< png_tag >()
    : image_read_settings_base()
    , _gamma( 1.0 )
    {}

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       , double         gamma = 1.0
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    , _gamma( gamma )
    {}

    png_gamma::type _gamma;
};

#else

template<>
struct image_read_settings< png_tag > : public image_read_settings_base
{
    image_read_settings< png_tag >()
    : image_read_settings_base()
    {
        init_members();
    }

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    {
        init_members();
    }

    void init_members()
    {
        _screen_gamma                    = 2.2;
        _read_cie_chromacities           = false;
        _read_icc_profile                = false;
        _read_intent                     = false;
        _read_palette                    = false;
        _read_background                 = false;
        _read_histogram                  = false;
        _read_screen_offsets             = false;
        _read_pixel_calibration          = false;
        _read_physical_resolution        = false;
        _read_number_of_significant_bits = false;
        _read_scale_factors              = false;
        _read_comments                   = false;
        _read_last_modification_time     = false;
        _read_transparency_data          = false;
    }

    void set_members_true()
    {
        _read_cie_chromacities           = true;
        _read_icc_profile                = true;
        _read_intent                     = true;
        _read_palette                    = true;
        _read_background                 = true;
        _read_histogram                  = true;
        _read_screen_offsets             = true;
        _read_pixel_calibration          = true;
        _read_physical_resolution        = true;
        _read_number_of_significant_bits = true;
        _read_scale_factors              = true;
        _read_comments                   = true;
        _read_last_modification_time     = true;
        _read_transparency_data          = true;
    }

    double _screen_gamma;

    bool _read_cie_chromacities;
    bool _read_icc_profile;
    bool _read_intent;
    bool _read_palette;
    bool _read_background;
    bool _read_histogram;
    bool _read_screen_offsets;
    bool _read_pixel_calibration;
    bool _read_physical_resolution;
    bool _read_number_of_significant_bits;
    bool _read_scale_factors;
    bool _read_comments;
    bool _read_last_modification_time;
    bool _read_transparency_data;
};
#endif

template<>
struct image_write_info<png_tag>
{
    image_write_info()
    : _interlace_method  ( PNG_INTERLACE_NONE           )
    , _compression_method( PNG_COMPRESSION_TYPE_DEFAULT )
    , _filter_method     ( PNG_FILTER_TYPE_DEFAULT      )

    , _gamma( 0 )
    , _x_res( 0 )
    , _y_res( 0 )
    , _sbits()
    {}

    png_interlace_method::type   _interlace_method;
    png_compression_method::type _compression_method;
    png_filter_method::type      _filter_method;

    png_gamma::type            _gamma;
    png_resolution::type       _x_res;
    png_resolution::type       _y_res;
    png_significant_bits::type _sbits;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP
