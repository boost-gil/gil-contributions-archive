/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "is_allowed.hpp"

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
    , _pitch( 0 )
    {}

    ~reader_backend()
    {
        _io_dev.set_close( true );
    }

    Device _io_dev;

    image_read_settings< pnm_tag > _settings;
    image_read_info< pnm_tag >     _info;

    std::size_t _pitch;
};

///
/// PNM Reader
///
template< typename Device >
class scanline_reader< Device
                     , pnm_tag
                     >
    : public reader_backend< Device
                           , pnm_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , pnm_tag
                           > this_t;

public:

    typedef reader_backend< Device
                          , pnm_tag
                          > backend_t;


public:
    scanline_reader( Device&                                device
                   , const image_read_settings< pnm_tag >& settings
                   )
    : reader_backend( device
                    , settings
                    )
    {}

    void read_header()
    {
        // read signature
        io_error_if( read_char() != 'P', "Invalid PNM signature" );

        this->_info._type = read_char() - '0';

		io_error_if( this->_info._type < pnm_image_type::mono_asc_t::value || this->_info._type > pnm_image_type::color_bin_t::value
		           , "Invalid PNM file (supports P1 to P6)"
		           );

        this->_info._width  = read_int();
        this->_info._height = read_int();

        if( this->_info._type == pnm_image_type::mono_asc_t::value || this->_info._type == pnm_image_type::mono_bin_t::value )
        {
            this->_info._max_value = 1;
        }
        else
        {
            this->_info._max_value = read_int();

		    io_error_if( this->_info._max_value > 255
		               , "Unsupported PNM format (supports maximum value 255)"
		               );
        }
    }

    void initialize()
    {
        switch( this->_info._type )
		{
            // reading mono text is reading grayscale but with only two values
			case pnm_image_type::mono_asc_t::value:  
			case pnm_image_type::gray_asc_t::value:
            {
                this->_pitch = this->_info._width;

                _read_function = boost::mem_fn( &this_t::read_text_image ); 

                break;
                }

			case pnm_image_type::color_asc_t::value:
            {
                this->_pitch = this->_info._width * num_channels< rgb8_view_t >::value;

                _read_function = boost::mem_fn( &this_t::read_text_image  ); 

                break;
            }


			case pnm_image_type::mono_bin_t::value:
            {
                //gray1_image_t::view_t

                this->_pitch = ( this->_info._width + 7 ) >> 3;

                _read_function = boost::mem_fn( &this_t::read_binary_bit_image ); 

                break;
            }

			case pnm_image_type::gray_bin_t::value:
            {
                // gray8_view_t
                this->_pitch = this->_info._width;

                _read_function = boost::mem_fn( &this_t::read_binary_byte_image ); 

                break;
            }

			case pnm_image_type::color_bin_t::value:
            {
                this->_pitch = this->_info._width * num_channels< rgb8_view_t >::value;

                _read_function = boost::mem_fn( &this_t::read_binary_byte_image ); 

                break;
            }

            default: { io_error( "Unsupported pnm file." ); break; }
		}
    }

    void clean_up() {}

    /// Read part of image defined by View and return the data.
    void read( byte_t* dst, int pos )
    {
        _read_function(this, dst);
    }

    /// Return length of scanline in bytes.
    std::size_t scanline_length()
    {
        return _pitch;
    }


private:

    void read_text_image( byte_t* dst )
    {
        read_text_row( dst, true );
    }

    void read_text_row( byte_t* row
                      , bool    process
                      )
    {
        static char buf[16];

        for( uint32_t x = 0; x < this->_pitch; ++x )
        {
            for( uint32_t k = 0; ; )
            {
				int ch = _io_dev.getc_unchecked();

				if( isdigit( ch ))
				{
                    buf[ k++ ] = static_cast< char >( ch );
				}
				else if( k )
				{
					buf[ k ] = 0;
					break;
				}
				else if( ch == EOF || !isspace( ch ))
				{
					return;
				}
            }

            if( process )
            {
                int value = atoi( buf );

                if( this->_info._max_value == 1 )
                {
                    // for pnm format 0 is white
                    row[x] = ( value != 0 )
                             ? 0
                             : 255;
                }
                else
                {
                    row[x] = static_cast< byte_t >( value );
                }
            }
        }
    }

    void read_binary_bit_image( byte_t* dst )
    {
        _io_dev.read( dst
                    , this->_pitch
                    );

        _negate_bits    ( dst, scanline_length() );
        _swap_half_bytes( dst, scanline_length() );
    }

    void read_binary_byte_image( byte_t* dst )
    {
        _io_dev.read( dst
                    , this->_pitch
                    );
    }


private:

    // Read a character and skip a comment if necessary.
    char read_char()
    {
        char ch;

        if(( ch = _io_dev.getc() ) == '#' )
        {
            // skip comment to EOL
            do
            {
                ch = _io_dev.getc();
            }
            while (ch != '\n' && ch != '\r');
        }

        return ch;
    }

	unsigned int read_int()
	{
		char ch;

        // skip whitespaces, tabs, and new lines
		do
		{
			ch = read_char();
		}
		while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

		if( ch < '0' || ch > '9' )
		{
			io_error( "Unexpected characters reading decimal digits" );
		}

		unsigned val = 0;

        do
        {
			unsigned dig = ch - '0';

			if( val > INT_MAX / 10 - dig )
			{
				io_error( "Integer too large" );
			}

			val = val * 10 + dig;

			ch = read_char();
		}
		while( '0' <= ch && ch <= '9' );

		return val;
	}

private:

    // For bit_aligned images we need to negate all bytes in the row_buffer
    // to make sure that 0 is black and 255 is white.
    detail::negate_bits    < std::vector< byte_t >, mpl::true_ > _negate_bits;
    detail::swap_half_bytes< std::vector< byte_t >, mpl::true_ > _swap_half_bytes;

    boost::function< void ( this_t*, byte_t* ) > _read_function;
};


} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP
