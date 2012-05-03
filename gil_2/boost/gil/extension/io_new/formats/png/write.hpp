/*
    Copyright 2007-2012 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_WRITE_HPP
#define BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_WRITE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

// taken from jpegxx - https://bitbucket.org/edd/jpegxx/src/ea2492a1a4a6/src/ijg_headers.hpp
#ifndef BOOST_GIL_EXTENSION_IO_PNG_C_LIB_COMPILED_AS_CPLUSPLUS
    extern "C" {
#else
    // DONT_USE_EXTERN_C introduced in v7 of the IJG library.
    // By default the v7 IJG headers check for __cplusplus being defined and
    // wrap the content in an 'extern "C"' block if it's present.
    // When DONT_USE_EXTERN_C is defined, this wrapping is not performed.
    #ifndef DONT_USE_EXTERN_C
        #define DONT_USE_EXTERN_C 1
    #endif
#endif

#include <png.h>

#ifndef BOOST_GIL_EXTENSION_IO_PNG_C_LIB_COMPILED_AS_CPLUSPLUS
    }
#endif

#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>

#include "writer_backend.hpp"

namespace boost { namespace gil {

namespace detail {

struct png_write_is_supported
{
    template< typename View >
    struct apply 
        : public is_write_supported< typename get_pixel_type< View >::type
                                   , png_tag
                                   >
    {};
};

} // namespace detail

///
/// PNG Writer
///
template< typename Device >
class writer< Device
            , png_tag
            > 
    : public writer_backend< Device
                           , png_tag
                           >
{

public:

    typedef writer_backend< Device , png_tag > backend_t;

    writer( const Device&                      io_dev
          , const image_write_info< png_tag >& info
          )
    : backend_t( io_dev
               , info
               )
    {}


    template< typename View >
    void apply( const View& view )
    {
        io_error_if( view.width() == 0 && view.height() == 0
                   , "png format cannot handle empty views."
                   );

        write_header( view );

        write_view( view
                  , typename is_bit_aligned< View >::type()
                  );
    }

private:

    template<typename View>
    void write_view( const View& view
                   ,  mpl::false_       // is bit aligned
                   )
    {
        typedef typename get_pixel_type< View >::type pixel_t;

        typedef detail::png_write_support< typename channel_type    < pixel_t >::type
                                         , typename color_space_type< pixel_t >::type
                                         > png_rw_info;

        if( little_endian() )
        {
            if( png_rw_info::_bit_depth == 16 )
            {
                png_set_swap( this->get_struct() );
            }

            if( png_rw_info::_bit_depth < 8 )
            {
                png_set_packswap( this->get_struct() );
            }
        }

        detail::row_buffer_helper_view< View > row_buffer( view.width()
                                                         , false
                                                         );

        for( int y = 0; y != view.height(); ++ y)
        {
            std::copy( view.row_begin( y )
                     , view.row_end  ( y )
                     , row_buffer.begin()
                     );

            png_write_row( this->get_struct()
                         , reinterpret_cast< png_bytep >( row_buffer.data() )
                         );
        }

        png_write_end( this->get_struct()
                     , this->get_info()
                     );
    }

    template<typename View>
    void write_view( const View& view
                   , mpl::true_         // is bit aligned
                   )
    {
        typedef detail::png_write_support< typename kth_semantic_element_type< typename View::value_type
                                                                             , 0
                                                                             >::type
                                         , typename color_space_type<View>::type
                                         > png_rw_info;

        if (little_endian() )
        {
            if( png_rw_info::_bit_depth == 16 )
            {
                png_set_swap( this->get_struct() );
            }

            if( png_rw_info::_bit_depth < 8 )
            {
                png_set_packswap( this->get_struct() );
            }
        }

        detail::row_buffer_helper_view< View > row_buffer( view.width()
                                                         , false
                                                         );

        for( int y = 0; y != view.height(); ++y )
        {
            std::copy( view.row_begin( y )
                     , view.row_end  ( y )
                     , row_buffer.begin()
                     );

            png_write_row( this->get_struct()
                         , reinterpret_cast< png_bytep >( row_buffer.data() )
                         );
        }

        png_free_data( this->get_struct()
                     , this->get_info()
                     , PNG_FREE_UNKN
                     , -1
                     );

        png_write_end( this->get_struct()
                     , this->get_info()
                     );
    }
};

///
/// PNG Dynamic Image Writer
///
template< typename Device >
class dynamic_image_writer< Device
                          , png_tag
                          >
    : public writer< Device
                   , png_tag
                   >
{
    typedef writer< Device
                  , png_tag
                  > parent_t;

public:

    dynamic_image_writer( const Device&                      io_dev
                        , const image_write_info< png_tag >& info
)
    : parent_t( io_dev
              , info
              )
    {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        detail::dynamic_io_fnobj< detail::png_write_is_supported
                                , parent_t
                                > op( this );

        apply_operation( views, op );
    }
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_WRITE_HPP
