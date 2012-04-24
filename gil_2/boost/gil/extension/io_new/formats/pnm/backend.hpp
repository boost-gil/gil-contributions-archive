/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_BACKEND_HPP
#define BOOST_GIL_EXTENSION_IO_PNM_IO_BACKEND_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/pnm_tags.hpp>

namespace boost { namespace gil {

///
/// PNM Backend
///
template< typename Device >
struct reader_backend< Device
                     , pnm_tag
                     >
{
    reader_backend( Device&                                 device
                  , const image_read_settings< pnm_tag >& settings
                  )
    : _io_dev  ( device   )
    , _settings( settings )
    , _info()

    , _scanline_length( 0 )
    {}

    ~reader_backend()
    {
        _io_dev.set_close( true );
    }

    void read_header()
    {
        // read signature
        io_error_if( read_char() != 'P', "Invalid PNM signature" );

        _info._type = read_char() - '0';

		io_error_if( _info._type < pnm_image_type::mono_asc_t::value || _info._type > pnm_image_type::color_bin_t::value
		           , "Invalid PNM file (supports P1 to P6)"
		           );

        _info._width  = read_int();
        _info._height = read_int();

        if( _info._type == pnm_image_type::mono_asc_t::value || _info._type == pnm_image_type::mono_bin_t::value )
        {
            _info._max_value = 1;
        }
        else
        {
            _info._max_value = read_int();

		    io_error_if( _info._max_value > 255
		               , "Unsupported PNM format (supports maximum value 255)"
		               );
        }
    }


public:

    Device _io_dev;

    image_read_settings< pnm_tag > _settings;
    image_read_info< pnm_tag >     _info;

    std::size_t _scanline_length;
};


} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_BACKEND_HPP
