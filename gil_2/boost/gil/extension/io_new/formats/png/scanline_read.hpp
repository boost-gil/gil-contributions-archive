/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP
#define BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/function.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>

#include "reader_backend.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil { 

///
/// PNG Reader
///
template< typename Device >
class scanline_reader< Device
                     , png_tag
                     >
    : public reader_backend< Device
                           , png_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , png_tag
                           > this_t;

public:

    typedef reader_backend< Device, png_tag > backend_t;

public:

    //
    // Constructor
    //
    scanline_reader( const Device&                         io_dev
                   , const image_read_settings< png_tag >& settings
                   )
    : reader_backend< Device
                    , png_tag
                    >( io_dev
                     , settings
                     )
    {}

    //
    // Destructor
    //
    ~scanline_reader()
    {
        png_destroy_read_struct( &this->_png_ptr
                               , &this->_info_ptr
                               , NULL
                               );
    }

    void initialize()
    {
        // Now it's time for some transformations.

        if( little_endian() )
        {
            if( this->_info._bit_depth == 16 )
            {
                // Swap bytes of 16 bit files to least significant byte first.
                png_set_swap( this->_png_ptr );
            }

            if( this->_info._bit_depth < 8 )
            {
                // swap bits of 1, 2, 4 bit packed pixel formats
                png_set_packswap( this->_png_ptr );
            }
        }

        if( this->_info._color_type == PNG_COLOR_TYPE_PALETTE )
        {
            png_set_palette_to_rgb( this->_png_ptr );
        }

        if( this->_info._num_trans > 0 )
        {
            png_set_tRNS_to_alpha( this->_png_ptr );
        }

        // Tell libpng to handle the gamma conversion for you.  The final call
        // is a good guess for PC generated images, but it should be configurable
        // by the user at run time by the user.  It is strongly suggested that
        // your application support gamma correction.
        if( this->_settings._apply_screen_gamma )
        {
            // png_set_gamma will change the image data!

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        png_set_gamma( _png_ptr
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#else
        png_set_gamma( this->_png_ptr
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        }

        // Turn on interlace handling.  REQUIRED if you are not using
        // png_read_image().  To see how to handle interlacing passes,
        // see the png_read_row() method below:
        this->_number_passes = png_set_interlace_handling( this->_png_ptr );
        io_error_if( this->_number_passes != 1, "scanline_read_iterator cannot read interlaced png images." );


        // The above transformation might have changed the bit_depth and color type.
        png_read_update_info( this->_png_ptr
                            , this->_info_ptr
                            );

        this->_info._bit_depth = png_get_bit_depth( this->_png_ptr
                                                  , this->_info_ptr
                                                  );

        this->_info._num_channels = png_get_channels( this->_png_ptr
                                                    , this->_info_ptr
                                                    );

        this->_info._color_type = png_get_color_type( this->_png_ptr
                                                    , this->_info_ptr
                                                    );

        this->_scanline_length = png_get_rowbytes( this->_png_ptr
                                                 , this->_info_ptr
                                                 );
    }

    void read( byte_t* dst, int pos )
    {
        read_scanline( dst );
    }

    /// Skip over a scanline.
    void skip( byte_t* dst, int )
    {
        read_scanline( dst );
    }

    void clean_up()
    {
        // read rest of file, and get additional chunks in info_ptr

        ///@todo
        //png_read_end( _png_ptr
        //            , NULL
        //            );
    }

private:

    void read_scanline( byte_t* dst )
    {
        png_read_rows( this->_png_ptr
                     , &dst
                     , NULL
                     , 1
                     );
    }


private:

    boost::function< void ( this_t*, byte_t* ) > _read_function;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_SCANLINE_READ_HPP
