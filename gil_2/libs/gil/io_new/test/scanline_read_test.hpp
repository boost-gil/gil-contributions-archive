#ifndef BOOST_GIL_IO_UNIT_TEST_SCANLINE_READ_TEST_HPP
#define BOOST_GIL_IO_UNIT_TEST_SCANLINE_READ_TEST_HPP

#include <boost/gil/gil_all.hpp>

#include "cmp_view.hpp"

inline
int align_to_four( int width )
{
    return ( width + 3 ) & ~3;
}

/// Also works with bit_aligned_images.
template< typename View >
inline
unsigned char* make_byte_t_ptr( View v )
{
    return &boost::gil::at_c<0>(*( v.row_begin( 0 )));
}

template< typename Image
        , typename FormatTag
        >
void test_scanline_reader( const char* file_name )
{

    // read image using scanline_read_iterator
    typedef scanline_reader< get_read_device< const char*, FormatTag >::type, FormatTag > reader_t;

    reader_t reader = make_scanline_reader( file_name, FormatTag() );

    Image dst   ( reader._info._width                , reader._info._height );
    Image buffer( align_to_four( reader._info._width),                    1 );

    auto it  = scanline_read_iterator< reader_t >( reader, make_byte_t_ptr( view( buffer )));
    auto end = scanline_read_iterator< reader_t >();

    int row = 0;
    while( it != end )
    {
        *it;

        copy_pixels( subimage_view( view( buffer ), 0,   0, reader._info._width, 1 )
                   , subimage_view( view( dst    ), 0, row, reader._info._width, 1 )
                   );

        ++row;
    }

    //compare
    Image img;
    read_image( file_name, img, FormatTag() );

    cmp_view( view( dst ), view( img ) );
}

#endif // BOOST_GIL_IO_UNIT_TEST_SCANLINE_READ_TEST_HPP
