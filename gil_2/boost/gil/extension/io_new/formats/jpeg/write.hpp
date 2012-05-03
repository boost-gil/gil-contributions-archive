/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITE_HPP
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <boost/gil/extension/io_new/jpeg_tags.hpp>

#include "supported_types.hpp"
#include "writer_backend.hpp"

namespace boost { namespace gil {

///
/// JPEG Writer
///
template< typename Device >
class writer< Device
            , jpeg_tag
            >
    : public writer_backend< Device
                           , jpeg_tag
                           >
{
public:

    typedef writer_backend< Device
                          , jpeg_tag
                          > backend_t;

public:

    writer( const Device&                       io_dev
          , const image_write_info< jpeg_tag >& info
          )
    : backend_t( io_dev
               , info
               )
    {}

    template<typename View>
    void apply( const View& view )
    {
        write_rows( view );
    }

private:

    template<typename View>
    void write_rows( const View& view )
    {
        std::vector< typename View::value_type > row_buffer( view.width() );

        // In case of an error we'll jump back to here and fire an exception.
        // @todo Is the buffer above cleaned up when the exception is thrown?
        //       The strategy right now is to allocate necessary memory before
        //       the setjmp.
        if( setjmp( _mark )) { raise_error(); }

        typedef typename channel_type< typename View::value_type >::type channel_t;

        get()->image_width      = JDIMENSION( view.width()  );
        get()->image_height     = JDIMENSION( view.height() );
        get()->input_components = num_channels<View>::value;
        get()->in_color_space   = detail::jpeg_write_support< channel_t
                                                                  , typename color_space_type< View >::type
                                                                  >::_color_space;

        jpeg_set_defaults( get() );

        jpeg_set_quality ( get()
                         , this->_info._quality
                         , TRUE
                         );

        // Needs to be done after jpeg_set_defaults() since it's overridding this value back to slow.
        get()->dct_method = this->_info._dct_method;


        // set the pixel dimensions
        get()->density_unit = this->_info._density_unit;
        get()->X_density    = this->_info._x_density;
        get()->Y_density    = this->_info._y_density;

        // done reading header information

        jpeg_start_compress( get()
                           , TRUE
                           );

        JSAMPLE* row_addr = reinterpret_cast< JSAMPLE* >( &row_buffer[0] );

        for( int y =0; y != view.height(); ++ y )
        {
            std::copy( view.row_begin( y )
                     , view.row_end  ( y )
                     , row_buffer.begin()
                     );

            jpeg_write_scanlines( get()
                                , &row_addr
                                , 1
                                );
        }
    }
};

///
/// JPEG Dyamic Image Writer
///
template< typename Device >
class dynamic_image_writer< Device
                          , jpeg_tag
                          >
    : public writer< Device
                   , jpeg_tag
                   >
{
    typedef writer< Device
                  , jpeg_tag
                  > parent_t;

public:

    dynamic_image_writer( const Device&                       io_dev
                        , const image_write_info< jpeg_tag >& info
                        )
    : parent_t( io_dev
              , info
              )
    {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        dynamic_io_fnobj< detail::jpeg_write_is_supported
                        , parent_t
                        > op( this );

        detail::apply_operation( views, op );
    }

    template< typename Views >
    void apply( const any_image_view  < Views    >& views
              , const image_write_info< jpeg_tag >& info
              )
    {
        dynamic_io_fnobj< detail::jpeg_write_is_supported
                        , parent_t
                        > op( this );

        detail::apply_operation( views, info, op );
    }
};

namespace detail { 

struct jpeg_write_is_supported
{
    template< typename View >
    struct apply
        : public is_write_supported< typename get_pixel_type< View >::type
                                   , jpeg_tag
                                   >
    {};
};

// unary application
template <typename Types, typename Tag, typename Bits, typename Op>
typename Op::result_type GIL_FORCEINLINE apply_operation_basec( const Bits& bits
                                                              , std::size_t index
                                                              , const image_write_info< Tag >& info
                                                              , Op op
                                                              )
{
    return detail::apply_operation_fwd_fn<mpl::size<Types>::value>().template applyc<Types>( bits
                                                                                           , index
                                                                                           , op
                                                                                           );
}

// unary application
template< typename Types
        , typename Info
        , typename Bits
        , typename Op
        >
typename Op::result_type GIL_FORCEINLINE apply_operation_base( Bits&       bits
                                                             , std::size_t index
                                                             , const Info& info
                                                             , Op op
                                                             )
{
    return detail::apply_operation_fwd_fn< mpl::size< Types >::value>().template apply< Types
                                                                                      >( bits
                                                                                       , index
                                                                                       , info
                                                                                       , op
                                                                                       );
}



/// \ingroup Variant
/// \brief Invokes a generic constant operation (represented as a binary function object) on two variants
template< typename Types1
        , typename Info
        , typename BinaryOp
        >
GIL_FORCEINLINE
typename BinaryOp::result_type apply_operation( const variant< Types1 >& arg1
                                              , const Info&              info
                                              , BinaryOp op
                                              )
{
    typename variant< Types1 >::base_t bits = arg1.bits();

    return apply_operation_base< Types1
                               , image_write_info< jpeg_tag >
                               >( bits
                                , arg1.index()
                                , info
                                , op
                                );
}

} // detail

} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_WRITE_HPP
