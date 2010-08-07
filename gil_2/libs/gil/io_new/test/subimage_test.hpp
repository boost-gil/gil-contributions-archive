#ifndef _GIL_IO_UNIT_TEST_SUBIMAGE_TEST_HPP_
#define _GIL_IO_UNIT_TEST_SUBIMAGE_TEST_HPP_

#include <boost/gil/gil_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

template< typename Image
        , typename Format
        >
void run_subimage_test( string filename
                      , const point_t& top_left
                      , const point_t& dimension
                      )
{
    Image original, subimage;

    read_image( filename
              , original
              , Format()
              );

    image_read_settings< Format > settings( top_left
                                           , dimension
                                           );


    read_image( filename
              , subimage
              , settings
              );

    BOOST_CHECK( equal_pixels( const_view( subimage )
                             , subimage_view( const_view( original )
                                            , top_left
                                            , dimension
                                            )
                             )
               );
}

#endif // _GIL_IO_UNIT_TEST_SUBIMAGE_TEST_HPP_
