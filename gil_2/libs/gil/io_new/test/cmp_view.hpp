#ifndef BOOST_GIL_IO_UNIT_TEST_CMP_VIEW_HPP
#define BOOST_GIL_IO_UNIT_TEST_CMP_VIEW_HPP

#include <boost/gil/gil_all.hpp>

template< typename View >
void cmp_view( const View& v1
             , const View& v2
             )
{
    if( v1.dimensions() != v2.dimensions() )
    {
        throw std::runtime_error( "Images are not equal." );
    }

    typename View::x_coord_t width  = v1.width();
    typename View::y_coord_t height = v1.height();

    for( typename View::y_coord_t y = 0; y < height; ++y )
    {
        const typename View::x_iterator src_it = v1.row_begin( y );
        const typename View::x_iterator dst_it = v2.row_begin( y );

        for( typename View::x_coord_t x = 0; x < width; ++x )
        {
            if( *src_it != *dst_it )
            {
                throw std::runtime_error( "Images are not equal." );
            }
        }
    }
}

#endif // BOOST_GIL_IO_UNIT_TEST_CMP_VIEW_HPP
