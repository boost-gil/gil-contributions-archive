/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP
#define BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <boost/gil/extension/io_new/bmp_tags.hpp>

#include "writer_backend.hpp"

namespace boost { namespace gil {

///
/// BMP Writer
///
template< typename Device >
class writer< Device
            , bmp_tag
            >
    : public writer_backend< Device
                           , bmp_tag
                           >
{
public:

    writer( const Device&                      io_dev
          , const image_write_info< bmp_tag >& info 
          )
    : writer_backend( io_dev
                    , info
                    )
    {}

    template<typename View>
    void apply( const View& view )
    {
        write( view );
    }

private:

    template< typename View >
    void write( const View& view )
    {
        typedef typename channel_type<
                    typename get_pixel_type< View >::type >::type channel_t;

        typedef typename color_space_type< View >::type color_space_t;

        // check if supported
/*
        /// todo
        if( bmp_read_write_support_private<channel_t, color_space_t>::channel != 8)
        {
            io_error("Input view type is incompatible with the image type");
        }
*/

        // compute the file size
        int bpp = num_channels< View >::value * 8;
        int entries = 0;

/*
        /// @todo: Not supported for now. bit_aligned_images refer to indexed images
        ///        in this context.
        if( bpp <= 8 )
        {
            entries = 1 << bpp;
        }
*/

        std::size_t spn = ( view.width() * num_channels< View >::value + 3 ) & ~3;
        std::size_t ofs = bmp_header_size::_size 
                        + bmp_header_size::_win32_info_size 
                        + entries * 4;

        std::size_t siz = ofs + spn * view.height();

        // write the BMP file header
        _io_dev.write_uint16( bmp_signature );
        _io_dev.write_uint32( (uint32_t) siz );
        _io_dev.write_uint16( 0 );
        _io_dev.write_uint16( 0 );
        _io_dev.write_uint32( (uint32_t) ofs );

        // writes Windows information header
        _io_dev.write_uint32( bmp_header_size::_win32_info_size );
        _io_dev.write_uint32( static_cast< uint32_t >( view.width()  ));
        _io_dev.write_uint32( static_cast< uint32_t >( view.height() ));
        _io_dev.write_uint16( 1 );
        _io_dev.write_uint16( static_cast< uint16_t >( bpp ));
        _io_dev.write_uint32( bmp_compression::_rgb );
        _io_dev.write_uint32( 0 );
        _io_dev.write_uint32( 0 );
        _io_dev.write_uint32( 0 );
        _io_dev.write_uint32( entries );
        _io_dev.write_uint32( 0 );

        write_image< View
                   , typename detail::get_bgr_cs< num_channels< View >::value >::type
                   >( view, spn );
    }


    template< typename View
            , typename BMP_View
            >
    void write_image( const View&       view
                    , const std::size_t spn
                    )
    {
        byte_vector_t buffer( spn );
        std::fill( buffer.begin(), buffer.end(), 0 );


        BMP_View row = interleaved_view( view.width()
                                       , 1
                                       , (typename BMP_View::value_type*) &buffer.front()
                                       , spn
                                       );

        for( typename View::y_coord_t y = view.height() - 1; y > -1; --y  )
        {
            copy_pixels( subimage_view( view
                                      , 0
                                      , (int) y
                                      , (int) view.width()
                                      , 1
                                      )
                       , row
                       );

            _io_dev.write( &buffer.front(), spn );
        }

    }
};

///
/// BMP Dynamic Image Writer
///
template< typename Device >
class dynamic_image_writer< Device
                          , bmp_tag
                          >
    : public writer< Device
                   , bmp_tag
                   >
{
    typedef writer< Device
                  , bmp_tag
                  > parent_t;

public:

    dynamic_image_writer( const Device&                      io_dev
                        , const image_write_info< bmp_tag >& info
                        )
    : parent_t( io_dev
              , info
              )
    {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        detail::dynamic_io_fnobj< detail::bmp_write_is_supported
                                , parent_t
                                > op( this );

        apply_operation( views
                       , op
                       );
    }
};

namespace detail {

struct bmp_write_is_supported
{
    template< typename View >
    struct apply
        : public is_write_supported< typename get_pixel_type< View >::type
                                   , bmp_tag
                                   >
    {};
};

template < int N > struct get_bgr_cs {};
template <> struct get_bgr_cs< 1 > { typedef gray8_view_t type; };
template <> struct get_bgr_cs< 3 > { typedef bgr8_view_t type;  };
template <> struct get_bgr_cs< 4 > { typedef bgra8_view_t type; };

} // namespace detail


} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP
