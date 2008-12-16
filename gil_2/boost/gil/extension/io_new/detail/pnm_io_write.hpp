/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_WRITE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNM_IO_WRITE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning \n
///         
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "base.hpp"
#include "io_device.hpp"

#include <boost/gil/extension/io_new/pnm_tags.hpp>

namespace boost { namespace gil { namespace detail {

template< typename Device >
class writer< Device
            , pnm_tag
            > 
{
    typedef image_write_info< pnm_tag > info_t;

public:

    writer( Device & file )
        : _out( file )
    {
    }

    ~writer()
    {
    }

    template<typename View>
    void apply( const View& view )
    {
        info_t info;

        apply( view
             , info );
    }

    template<typename View>
    void apply( const View&   view
              , const info_t& info )
    {
        int width  = view.width();
        int height = view.height();

        int num    = num_channels< View >::value;
        int chn    = std::min( 3, num );
        int bpp    = chn * 8;
        int pitch  = chn * width;
        int type   = ( num_channels<View>::value == 1 ) ? pnm_type_gray_bin : pnm_type_color_bin;


        // write header

        // Add a white space at each string so read_int() can decide when a numbers ends.
        char buffer[10];

        std::string str( "P" );
        _itoa( type, buffer, 10 );
        str += buffer + std::string( " " );
        _out.print_line( str );

        str.clear();
        _itoa( width, buffer, 10 );
        str += buffer + std::string( " " );
        _out.print_line( str );

        str.clear();
        _itoa( height, buffer, 10 );
        str += buffer + std::string( " " );
        _out.print_line( str );

        _out.print_line( "255 ");


        // write data

		std::vector<byte_t> buf( pitch );

        View row = interleaved_view( width
                                   , 1
                                   , (typename View::value_type*) &buf.front()
                                   , pitch
                                   );


		for( int y = 0; y < height; ++y )
		{
            copy_pixels( subimage_view( view, 0, y, view.width(), 1 )
                       , row
                       );

            _out.write( &buf.front(), pitch );
		}
    }

private:

    Device& _out;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITE_HPP_INCLUDED
