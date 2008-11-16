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
    : public reader_base< pnm_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:
    reader( Device& device )
    : _io_dev( device )
    {}

    reader( Device&     device
          , const cc_t& cc
          )
    : reader_base< pnm_tag
                 , ConversionPolicy >( cc )
    , _io_dev( device )
    {}

    image_read_info<pnm_tag> get_info()
    {
        image_read_info<pnm_tag> ret;

        // read signature
        io_error_if( read_char() != 'P' );

        ret._type = read_char() - '0';

		io_error_if( ret._type < type_mono_asc || type > type_color_bin
		           , "Invalid PNM file (supports P1 to P6)"
		           );

        ret._width  = read_int();
        ret._height = read_int();

        return ret;
    }

    template<typename View>
    void apply( const View& view )
    {
    }

private:

    // Read a character and skip a comment if necessary.
    char read_char()
    {
        char ch;

        if(( type = _io_dev.getc() ) == '#' )
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
			ch = _io_dev.read_char();
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

    Device& _io_dev;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED
