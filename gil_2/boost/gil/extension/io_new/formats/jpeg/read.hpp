/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <csetjmp>
#include <vector>
#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "base.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil {

///
/// JPEG Reader
///
template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , jpeg_tag
            , ConversionPolicy
            >
    : public reader_base< jpeg_tag
                        , ConversionPolicy
                        >
    , public reader_backend< Device
                           , jpeg_tag
                           >
{
private:

    typedef reader< Device
                  , bmp_tag
                  > this_t;

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    typedef reader_backend< Device, bmp_tag > backend_t;

public:

    //
    // Constructor
    //
    reader( Device&                                device
          , const image_read_settings< jpeg_tag >& settings
          )
    : reader_base< jpeg_tag
                 , ConversionPolicy >( settings )

    , backend_t( device
               , settings
               )
    {}

    //
    // Constructor
    //
    reader( Device&                                                device
          , const typename ConversionPolicy::color_converter_type& cc
          , const image_read_settings< jpeg_tag >&                 settings
          )
    : reader_base< jpeg_tag
                 , ConversionPolicy >( cc
                                     , settings
                                     )
    , backend_t( device
               , settings
               )
    {}

    image_read_info< jpeg_tag > get_info()
    {
        image_read_info<jpeg_tag> ret;
        ret._width          = this->_cinfo.image_width;
        ret._height         = this->_cinfo.image_height;
        ret._num_components = this->_cinfo.num_components;
        ret._color_space    = this->_cinfo.jpeg_color_space;
        ret._data_precision = this->_cinfo.data_precision;

        ret._density_unit = this->_cinfo.density_unit;
        ret._x_density    = this->_cinfo.X_density;
        ret._y_density    = this->_cinfo.Y_density;

        // obtain real world dimensions
        // taken from https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/read.cpp#cl-62
        jpeg_calc_output_dimensions( &this->_cinfo );

        double units_conversion = 0;
        if (this->_cinfo.density_unit == 1) // dots per inch
        {
            units_conversion = 25.4; // millimeters in an inch
        }
        else if (this->_cinfo.density_unit == 2) // dots per cm
        {
            units_conversion = 10; // millimeters in a centimeter
        }

        ret._pixel_width_mm  = this->_cinfo.X_density ? (this->_cinfo.output_width  / double(this->_cinfo.X_density)) * units_conversion : 0;
        ret._pixel_height_mm = this->_cinfo.Y_density ? (this->_cinfo.output_height / double(this->_cinfo.Y_density)) * units_conversion : 0;

        return ret;
    }

    template<typename View>
    void apply( const View& view )
    {
        // Fire exception in case of error.
        if( setjmp( this->_mark )) { this->raise_error(); }

        jpeg_decompress_struct& cinfo = this->_cinfo;
        cinfo.dct_method = this->_settings._dct_method;

        typedef typename is_same< ConversionPolicy
                                , read_and_no_convert
                                >::type is_read_and_convert_t;

        io_error_if( !is_allowed< View >( this->_info
                                        , is_read_and_convert_t()
                                        )
                   , "Image types aren't compatible."
                   );

        if( jpeg_start_decompress( &this->_cinfo ) == false )
        {
            io_error( "Cannot start decompression." );
        }

        switch( this->_info._color_space )
        {
            case JCS_GRAYSCALE: { read_rows< gray8_pixel_t >( view ); break; }
            case JCS_RGB:       { read_rows< rgb8_pixel_t  >( view ); break; }

            //!\todo add Y'CbCr? We loose image quality when reading JCS_YCbCr as JCS_RGB
            case JCS_YCbCr:     { read_rows< rgb8_pixel_t  >( view ); break; }

            case JCS_CMYK:      { read_rows< cmyk8_pixel_t >( view ); break; }

            //!\todo add Y'CbCrK? We loose image quality when reading JCS_YCCK as JCS_CMYK
            case JCS_YCCK:
            {
                this->_cinfo.out_color_space = JCS_CMYK;
                read_rows< cmyk8_pixel_t >( view );

                break;
            }
            default: { io_error( "Unsupported jpeg color space." ); }
        }

        jpeg_finish_decompress ( &this->_cinfo );
    }

private:

    template< typename ImagePixel
            , typename View
            >
    void read_rows( const View& view )
    {
        typedef std::vector<ImagePixel> buffer_t;
        buffer_t buffer( this->_info._width );

        // In case of an error we'll jump back to here and fire an exception.
        // @todo Is the buffer above cleaned up when the exception is thrown?
        //       The strategy right now is to allocate necessary memory before
        //       the setjmp.
        if( setjmp( this->_mark )) { this->raise_error(); }


        JSAMPLE *row_adr = reinterpret_cast< JSAMPLE* >( &buffer[0] );

        //Skip scanlines if necessary.
        for( int y = 0; y <  this->_settings._top_left.y; ++y )
        {
            io_error_if( jpeg_read_scanlines( &this->_cinfo
                                         , &row_adr
                                         , 1
                                         ) !=1
                       , "jpeg_read_scanlines: fail to read JPEG file"
                       );
        }

        // Read data.
        for( int y = 0; y < view.height(); ++y )
        {
            io_error_if( jpeg_read_scanlines( &this->_cinfo
                                         , &row_adr
                                         , 1
                                         ) !=1
                       , "jpeg_read_scanlines: fail to read JPEG file"
                       );

            typename buffer_t::iterator beg = buffer.begin() + this->_settings._top_left.x;
            typename buffer_t::iterator end = beg + this->_settings._dim.x;

            this->_cc_policy.read( beg
                                 , end
                                 , view.row_begin( y )
                                 );
        }

        //@todo: There might be a better way to do that.
        while( this->_cinfo.output_scanline <  this->_cinfo.image_height )
        {
            io_error_if( jpeg_read_scanlines( &this->_cinfo
                                            , &row_adr
                                            , 1
                                            ) !=1
                       , "jpeg_read_scanlines: fail to read JPEG file"
                       );
        }

    }
};

///
/// JPEG Dynamic Reader
///
template< typename Device >
class dynamic_image_reader< Device
                          , jpeg_tag
                          >
    : public reader< Device
                   , jpeg_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , jpeg_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                                device
                        , const image_read_settings< jpeg_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        jpeg_type_format_checker format_checker( this->_info._color_space != JCS_YCbCr
                                               ? this->_info._color_space
                                               : JCS_RGB
                                               );

        if( !construct_matched( images
                              , format_checker
                              ))
        {
            io_error( "No matching image type between those of the given any_image and that of the file" );
        }
        else
        {
            init_image( images
                      , this->_info
                      );

            dynamic_io_fnobj< jpeg_read_is_supported
                            , parent_t
                            > op( this );

            apply_operation( view( images )
                           , op
                           );
        }
    }
};

namespace detail { 

struct jpeg_type_format_checker
{
    jpeg_type_format_checker( jpeg_color_space::type color_space )
    : _color_space( color_space )
    {}

    template< typename Image >
    bool apply()
    {
        return is_read_supported< typename get_pixel_type< typename Image::view_t >::type
                                , jpeg_tag
                                >::_color_space == _color_space;
    }

private:

    jpeg_color_space::type _color_space;
};

struct jpeg_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , jpeg_tag
                                           >
    {};
};

} // detail

} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_READ_HPP
