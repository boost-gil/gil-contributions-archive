/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <boost/gil/extension/io_new/bmp_tags.hpp>

namespace boost { namespace gil { namespace detail {

template < int N > struct get_bgr_cs {};
template <> struct get_bgr_cs< 1 > { typedef gray8_view_t type; };
template <> struct get_bgr_cs< 3 > { typedef bgr8_view_t type; };
template <> struct get_bgr_cs< 4 > { typedef bgra8_view_t type; };

template< typename Device >
class writer< Device
            , bmp_tag
            > 
{
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
        write( view );
    }

    template<typename View>
    void apply( const View&                        view
              , const image_write_info< bmp_tag >& info )
    {
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

        int spn = ( view.width() * num_channels< View >::value + 3 ) & ~3;
        int ofs = bmp_header_size + bmp_win32_info_size + entries * 4;
        int siz = ofs + spn * view.height();

        // write the BMP file header
        _out.write_int16( bmp_signature );
        _out.write_int32( siz );
        _out.write_int16( 0 );
        _out.write_int16( 0 );
        _out.write_int32( ofs );

        // writes Windows information header
        _out.write_int32( bmp_win32_info_size );
        _out.write_int32( view.width()  );
        _out.write_int32( view.height() );
        _out.write_int16( 1 );
        _out.write_int16( bpp );
        _out.write_int32( ct_rgb );
        _out.write_int32( 0 );
        _out.write_int32( 0 );
        _out.write_int32( 0 );
        _out.write_int32( entries );
        _out.write_int32( 0 );

        write_image< View
                   , get_bgr_cs< num_channels< View >::value >::type
                   >( view, spn );
    }


    template< typename View
            , typename BMP_View
            >
    void write_image( const View&       view
                    , const std::size_t spn
                    )
    {
        std::vector< byte_t > buffer( spn );
        std::fill( buffer.begin(), buffer.end(), 0 );


        BMP_View row = interleaved_view( view.width()
                                       , 1
                                       , (typename BMP_View::value_type*) &buffer.front()
                                       , spn
                                       );

        for( int y = view.height() - 1; y > -1; --y  )
        {
            copy_pixels( subimage_view( view, 0, y, view.width(), 1 )
                       , row
                       );

            _out.write( &buffer.front(), spn );
        }

    }

private:

    Device& _out;
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_BMP_IO_WRITE_HPP_INCLUDED
