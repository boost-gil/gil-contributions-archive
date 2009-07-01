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

struct png_background : property_base< png_color_16 > {};

struct png_histrogram : property_base< std::vector< png_uint_16 > > {};

struct png_offset      : property_base< png_int_32 > {};
struct png_offset_type : property_base< int > {};

struct png_CAL        : property_base< std::string > {};
struct png_CAL_params : property_base< std::vector< std::string > > {};
struct png_CAL_X      : property_base< png_int_32 > {};
struct png_CAL_type   : property_base< int > {};
struct png_CAL_nparam : property_base< int > {};

struct png_resolution : property_base< png_uint_32 > {};
struct png_unit_type  : property_base< int > {};

struct png_significant_bits : property_base< png_color_8 > {};

struct gil_io_png_text
{
    int         _compression;
    std::string _key;
    std::string _text;
};

struct png_text_    : property_base< std::vector< gil_io_png_text > > {};
struct png_num_text : property_base< int > {};

struct png_mod_time : property_base< png_time > {};

struct png_trans        : property_base< std::vector< png_byte > > {};
struct png_num_trans    : property_base< int > {};
struct png_trans_values : property_base< std::vector< png_color_16 > > {};

struct png_return_value : property_base< png_uint_32 > {};

struct png_info_base
{
    png_info_base()
    : _width ( 0 )
    , _height( 0 )

    , _bit_depth         ( 0                            )
    , _color_type        ( 0                            )
    , _interlace_method  ( PNG_INTERLACE_NONE           )
    , _compression_method( PNG_COMPRESSION_TYPE_DEFAULT )
    , _filter_method     ( PNG_FILTER_TYPE_DEFAULT      )

    , _num_channels( 0 )

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
    , _valid_cie_colors( 0   )
    , _white_x         ( 0.0 )
    , _white_y         ( 0.0 )
    , _red_x           ( 0.0 )
    , _red_y           ( 0.0 )
    , _green_x         ( 0.0 )
    , _green_y         ( 0.0 )
    , _blue_x          ( 0.0 )
    , _blue_y          ( 0.0 )

    , _valid_file_gamma( 0   )
    , _file_gamma      ( 1.0 )
#else 
    , _valid_cie_colors( 0 )
    , _white_x         ( 0 )
    , _white_y         ( 0 )
    , _red_x           ( 0 )
    , _red_y           ( 0 )
    , _green_x         ( 0 )
    , _green_y         ( 0 )
    , _blue_x          ( 0 )
    , _blue_y          ( 0 )

    , _valid_file_gamma( 0 )
    , _file_gamma      ( 1 )
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

    , _valid_icc_profile    ( 0                         )
    , _icc_name             (                           )
    , _iccp_compression_type( PNG_COMPRESSION_TYPE_BASE )
    , _profile              (                           )
    , _profile_length       ( 0                         )

    , _valid_intent( 0 )
    , _intent      ( 0 )

    , _valid_palette( 0 )
    , _palette      (   )
    , _num_palette  ( 0 )

    , _valid_background( 0 )
    , _background      (   )

    , _valid_histogram( 0 )
    , _histogram      (   )

    , _valid_offset ( 0                )
    , _offset_x     ( 0                )
    , _offset_y     ( 0                )
    , _off_unit_type( PNG_OFFSET_PIXEL )

    , _valid_pixel_calibration( 0 )
    , _purpose                (   )
    , _X0                     ( 0 )
    , _X1                     ( 0 )
    , _cal_type               ( 0 )
    , _num_params             ( 0 )
    , _units                  (   )
    , _params                 (   )

    , _valid_resolution( 0                      )
    , _res_x           ( 0                      )
    , _res_y           ( 0                      )
    , _phy_unit_type   ( PNG_RESOLUTION_UNKNOWN )

    , _valid_significant_bits( 0 )
    , _sig_bits              (   )

    , _valid_scale_factors( 0                 )
    , _scale_unit         ( PNG_SCALE_UNKNOWN )
#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
    , _scale_width ( 0.0 )
    , _scale_height( 0.0 )
#else
    , _scale_width ()
    , _scale_height()
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

    , _valid_text( 0 )
    , _text      (   )
    , _num_text  ( 0 )

    , _valid_modification_time( 0 )
    , _mod_time               (   )

    , _valid_transparency_factors( 0 )
    , _trans                     (   )
    , _num_trans                 ( 0 )
    , _trans_values              (   )
    {}

    png_image_width::type  _width;
    png_image_height::type _height;

    png_bitdepth::type           _bit_depth;
    png_color_type::type         _color_type;
    png_interlace_method::type   _interlace_method;
    png_compression_method::type _compression_method;
    png_filter_method::type      _filter_method;

    png_num_channels::type _num_channels;

    // CIE chromacities
    png_return_value::type      _valid_cie_colors;
    png_chromacities_type::type _white_x;
    png_chromacities_type::type _white_y;
    png_chromacities_type::type _red_x;
    png_chromacities_type::type _red_y;
    png_chromacities_type::type _green_x;
    png_chromacities_type::type _green_y;
    png_chromacities_type::type _blue_x;
    png_chromacities_type::type _blue_y;

    // Gamma Value
    png_return_value::type _valid_file_gamma;
    png_gamma::type        _file_gamma;

    // Embedded ICC profile
    png_return_value::type         _valid_icc_profile;
    png_ICC_name::type             _icc_name;
    png_ICC_compression_type::type _iccp_compression_type;
    png_ICC_profile::type          _profile;
    png_ICC_profile_length::type   _profile_length;

    // Rendering intent
    png_return_value::type _valid_intent;
    png_intent::type       _intent;

    // Image palette
    png_return_value::type  _valid_palette;
    png_color_palette::type _palette;
    png_num_palette::type   _num_palette;

    // Background
    png_return_value::type _valid_background;
    png_background::type   _background;

    // Histogram
    png_return_value::type _valid_histogram;
    png_histrogram::type   _histogram;

    // Screen offsets
    png_return_value::type _valid_offset;
    png_offset::type       _offset_x;
    png_offset::type       _offset_y;
    png_offset_type::type  _off_unit_type;

    // Pixel Calibration
    png_return_value::type _valid_pixel_calibration;
    png_CAL::type          _purpose;
    png_CAL_X::type        _X0;
    png_CAL_X::type        _X1;
    png_CAL_type::type     _cal_type;
    png_CAL_nparam::type   _num_params;
    png_CAL::type          _units;
    png_CAL_params::type   _params;

    // Physical resolution
    png_return_value::type _valid_resolution;
    png_resolution::type   _res_x;
    png_resolution::type   _res_y;
    png_unit_type::type    _phy_unit_type;

    // Number of significant bits
    png_return_value::type     _valid_significant_bits;
    png_significant_bits::type _sig_bits;

    // Scale Factors
    png_return_value::type _valid_scale_factors;
    png_unit::type         _scale_unit;
    png_scale::type        _scale_width;
    png_scale::type        _scale_height;

    // Comments information
    png_return_value::type _valid_text;
    png_text_::type        _text;
    png_num_text::type     _num_text;

    // Last modification time
    png_return_value::type _valid_modification_time;
    png_mod_time::type     _mod_time;

    // Transparency data
    png_return_value::type _valid_transparency_factors;
    png_trans::type        _trans;
    png_num_trans::type    _num_trans;
    png_trans_values::type _trans_values;
};

template<>
struct image_read_info< png_tag > : public png_info_base
{
    image_read_info< png_tag >()
    : png_info_base()
    {}
};

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

template<>
struct image_read_settings< png_tag > : public image_read_settings_base
{
    image_read_settings< png_tag >()
    : image_read_settings_base()
    , _screen_gamma( 1.0 )
    {}

    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       , double         gamma = 1.0
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    , _screen_gamma( screen_gamma )
    {}

    png_gamma::type _screen_gamma;
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
        _apply_screen_gamma              = false;
        _screen_gamma                    = 2.2;

        _read_cie_chromacities           = false;
        _read_file_gamma                 = false;
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

    void set_read_members_true()
    {
        _read_cie_chromacities           = true;
        _read_file_gamma                 = true;
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

    bool   _apply_screen_gamma;
    double _screen_gamma;

    bool _read_cie_chromacities;
    bool _read_file_gamma;
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
struct image_write_info< png_tag >  : public png_info_base
{
    image_write_info()
    : png_info_base()
    {}
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNG_TAGS_HPP
