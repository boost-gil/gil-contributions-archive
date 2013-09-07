
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/toolbox/color_spaces/ycbcr.hpp>

#include <boost/gil/extension/yuv/yuv_planar_pixel_iterator.hpp>
#include <boost/gil/extension/yuv/yuv_image.hpp>

#include <boost/gil/extension/io/bmp_all.hpp>
#include <boost/gil/extension/io/png_all.hpp>


using namespace std;
using namespace boost;
using namespace gil;

const string yuv_image_path = "C:/gil_contributions/test_images/yuv/";
const string out_path = "C:/gil_contributions/output/yuv/";

BOOST_AUTO_TEST_SUITE( gil_yuv_tests )

BOOST_AUTO_TEST_CASE( x_iterator_test )
{
    typedef unsigned char* channel_ptr_t;
    typedef mpl::vector3<red_t,green_t,blue_t> color_space_t;
    typedef yuv_planar_pixel_iterator< channel_ptr_t, color_space_t > x_iterator;

    // three planes
    // image: 640 x 640
    int N = 640;
    vector< unsigned char > image( N * N + N * N / 2  );

    channel_ptr_t y_base = &image.front();
    channel_ptr_t v_base = y_base + ( N * N );
    channel_ptr_t u_base = v_base + N;

    x_iterator it( y_base, v_base, u_base );
    it++;

    assert( at_c<0>( it ) == y_base + 1 );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it--;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it += 7;

    assert( at_c<0>( it ) == y_base + 7 );
    assert( at_c<1>( it ) == v_base + 3 );
    assert( at_c<2>( it ) == u_base + 3 );

    it -= 7;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );

    it += 8;

    assert( at_c<0>( it ) == y_base + 8 );
    assert( at_c<1>( it ) == v_base + 4 );
    assert( at_c<2>( it ) == u_base + 4 );

    it -= 8;

    assert( at_c<0>( it ) == y_base );
    assert( at_c<1>( it ) == v_base );
    assert( at_c<2>( it ) == u_base );
}

typedef struct sDumpHeader {
  int xres;
  int yres;
  int pixelFormat;
  int depth;
  int flipY;
  int clipID;
  int reference;
} DumpHeader;

BOOST_AUTO_TEST_CASE( algorithm_test )
{
    // construct a view type
    typedef ycbcr_601_8_pixel_t pixel_t;
    typedef mpl::vector_c< int, 4, 2, 0 > factors_t;
    typedef get_yuv_view< pixel_t, factors_t >::type view_t;

    // allocate some memory and create a view
    std::size_t width  = 640;
    std::size_t height = 480; 

    vector< unsigned char > data( width * height  * 3 );

    view_t v = yuv_view< pixel_t
                       , factors_t
                       , boost::gil::IMC2
                       >( width
                        , height
                        , &data.front()
                        , width
                        );

    // memory layout looks like: yyyyuvuv

    // fill planes with some value
    fill_pixels( v, pixel_t( 0, 0, 0 ));
}


BOOST_AUTO_TEST_CASE( view_test )
{
    typedef unsigned char* channel_ptr_t;
    typedef ycbcr_601__t color_space_t;
    typedef yuv_planar_pixel_iterator< channel_ptr_t, color_space_t > x_iterator;

    // three planes
    // image: 640 x 640
    int N = 640;
    vector< unsigned char > image( N * N + N * N / 2  );

    channel_ptr_t y_base = &image.front();
    channel_ptr_t v_base = y_base + ( N * N );
    channel_ptr_t u_base = v_base + N;

    typedef type_from_x_iterator< yuv_planar_pixel_iterator< channel_ptr_t
                                                           , color_space_t
                                                           >
                                >::view_t view_t;

    //view_t v = planar_yuv_view< channel_ptr_t
    //                          , color_space_t
    //                          >( N, N, y_base, v_base, u_base, N );

    //auto raw_plane_0 = planar_view_get_raw_data( v, 0 );
    //auto raw_plane_1 = planar_view_get_raw_data( v, 1 );
    //auto raw_plane_2 = planar_view_get_raw_data( v, 2 );

    //assert( y_base == raw_plane_0 );
    //assert( v_base == raw_plane_1 );
    //assert( u_base == raw_plane_2 );

    //typedef kth_channel_view_type< 0, view_t >::type plane_t;
    //plane_t plane_view_0 = kth_channel_view<0>( v );
    //plane_t plane_view_1 = kth_channel_view<1>( v );
    //plane_t plane_view_2 = kth_channel_view<2>( v );

    //typedef view_t::value_type pixel_t;
    //pixel_t p = pixel_t( 200, 100, 50 );

    //fill_pixels(v, p);

}

BOOST_AUTO_TEST_CASE( image_read_test_rgb )
{
    {
        string filename = yuv_image_path + "tulips_rgb444_prog_packed_qcif.yuv";

        ifstream in( filename, std::fstream::binary );

        if( in == false )
        {
            return;
        }


        in.seekg(0, in.end);
        ifstream::pos_type length = in.tellg();
        in.seekg(0, in.beg);

        vector< unsigned char > data( (vector< unsigned char >::size_type) length );
        in.read( (char*) &data.front(), length );
        typedef rgb8_image_t image_t;
        typedef image_t::view_t view_t;

        view_t src = interleaved_view( 176, 144, (view_t::x_iterator) &data.front(), 176 * 3 );

        write_view( out_path + "rgb444_prog_packed.png", src, png_tag());
    }

    {
        string filename = yuv_image_path + "tulips_bgr444_prog_packed_qcif.yuv";

        ifstream in( filename, std::fstream::binary );

        if( in == false )
        {
            return;
        }


        in.seekg(0, in.end);
        ifstream::pos_type length = in.tellg();
        in.seekg(0, in.beg);

        vector< unsigned char > data( (vector< unsigned char >::size_type) length );
        in.read( (char*) &data.front(), length );
        typedef bgr8_image_t image_t;
        typedef image_t::view_t view_t;

        view_t src = interleaved_view( 176, 144, (view_t::x_iterator) &data.front(), 176 * 3 );

        write_view( out_path + "bgr444_prog_packed.png", src, png_tag());
    }

    {
        string filename = yuv_image_path + "tulips_rgb444_prog_planar_qcif.yuv";

        ifstream in( filename, std::fstream::binary );

        if( in == false )
        {
            return;
        }


        in.seekg(0, in.end);
        ifstream::pos_type length = in.tellg();
        in.seekg(0, in.beg);

        vector< unsigned char > data( (vector< unsigned char >::size_type) length );
        in.read( (char*) &data.front(), length );

        unsigned char* red   = &data.front();
        unsigned char* green = red + 176 * 144;
        unsigned char* blue  = green + 176 * 144;

        typedef rgb8_planar_image_t image_t;
        typedef image_t::view_t view_t;

        view_t src = planar_rgb_view( 176, 144, red, green, blue, 176 );

        write_view( out_path + "rgb444_prog_planar.png", src, png_tag());
    }

    {
        string filename = yuv_image_path + "tulips_bgr444_prog_planar_qcif.yuv";

        ifstream in( filename, std::fstream::binary );

        if( in == false )
        {
            return;
        }


        in.seekg(0, in.end);
        ifstream::pos_type length = in.tellg();
        in.seekg(0, in.beg);

        vector< unsigned char > data( (vector< unsigned char >::size_type) length );
        in.read( (char*) &data.front(), length );

        unsigned char* blue   = &data.front();
        unsigned char* green = blue + 176 * 144;
        unsigned char* red  = green + 176 * 144;

        typedef rgb8_planar_image_t image_t;
        typedef image_t::view_t view_t;

        view_t src = planar_rgb_view( 176, 144, red, green, blue, 176 );

        write_view( out_path + "bgr444_prog_planar.png", src, png_tag());
    }
}

BOOST_AUTO_TEST_CASE( image_read_test_yuv )
{
    {
        string filename = yuv_image_path + "tulips_yuv444_inter_packed_qcif.yuv";

        ifstream in( filename, std::fstream::binary );

        if( in == false )
        {
            return;
        }


        in.seekg(0, in.end);
        ifstream::pos_type length = in.tellg();
        in.seekg(0, in.beg);

        vector< unsigned char > data( (vector< unsigned char >::size_type) length );
        in.read( (char*) &data.front(), length );
        typedef ycbcr_601_8_image_t image_t;
        typedef image_t::view_t view_t;

        view_t src = interleaved_view( 176, 144, (view_t::x_iterator) &data.front(), 176 * 3 );

        typedef rgb8_image_t dst_image_t;
        typedef dst_image_t::view_t dst_view_t;

        dst_image_t dst_img( 176, 144 );

        copy_pixels( color_converted_view< rgb8_pixel_t >( src ), view( dst_img ));

        write_view( out_path + "yuv444_inter_packed.png", view( dst_img ), png_tag());
    }
}

BOOST_AUTO_TEST_CASE( subimage_test )
{
    //rgb8_planar_image_t img( 640, 480 );
    //rgb8_planar_image_t::view_t vv1 = view( img );

    //unsigned char* p1 = gil::at_c< 0 >( vv1.at( 0, 0 ).x() );

    //rgb8_planar_image_t::view_t vv2 = subimage_view( vv1, point_t( 10, 10 ), point_t( 320, 240 ));

    //rgb8_planar_image_t img2( 320, 240 );
    //copy_pixels( vv2, view( img2 ));


    //string filename = "C:/chhenning/yuv/videodump.raw";

    //ifstream in( filename, std::fstream::binary );

    //if( in == false )
    //{
    //    return;
    //}
    //

    //DumpHeader header;
    //std::streamsize header_size = sizeof( DumpHeader );

    //in.read( (char*) &header, header_size);

    //std::size_t width  = header.xres;
    //std::size_t height = header.yres;    

    //std::streamsize y_plane_size = width * height;
    //std::streamsize u_plane_size = ( width / 2 ) * ( height / 2 );
    //std::streamsize v_plane_size = ( width / 2 ) * ( height / 2 );

    //std::streamsize image_size = y_plane_size + u_plane_size + v_plane_size;

    //vector< unsigned char > data( (unsigned int) image_size );

    //rgb8_image_t image( 420
    //                  , 240
    //                  );

    //in.read( (char*) &data.front(), image_size);

    //typedef ycbcr_601_8_pixel_t pixel_t;
    //typedef mpl::vector_c< int, 4, 2, 0 > factors_t;
    //typedef get_yuv_view< pixel_t, factors_t >::type view_t;

    //    
    //view_t v = yuv_view< pixel_t
    //                    , factors_t
    //                    , boost::gil::IMC2
    //                    >( width
    //                    , height
    //                    , &data.front()
    //                    , width
    //                    );

    //view_t v2 = subimage_view( v, point_t( 10, 10 ), point_t( 420, 240 ));

    //copy_pixels( color_converted_view< bgr8_pixel_t >( v2 ), view( image ));

    //stringstream ss;
    //ss << "c:/chhenning/yuv/data/test_yuv420__" << 0 << ".png";

    //write_view( ss.str(), view( image ), png_tag() );
}

BOOST_AUTO_TEST_CASE( image_read_test_yuv_444 )
{
    string filename = yuv_image_path + "tulips_yuv444_prog_planar_qcif.yuv";

    ifstream in( filename, std::fstream::binary );

    if( in == false )
    {
        return;
    }
    
    std::size_t width  = 176;
    std::size_t height = 144;

    std::streamsize y_plane_size = width * height;
    std::streamsize u_plane_size = width * height;
    std::streamsize v_plane_size = width * height;

    std::streamsize image_size = y_plane_size + u_plane_size + v_plane_size;

    vector< unsigned char > data( (unsigned int) image_size );

    rgb8_image_t image( width, height );

    for( int i = 0; i < 6; ++i )
    {
        in.read( (char*) &data.front(), image_size);

        typedef ycbcr_601_8_pixel_t pixel_t;
        typedef mpl::vector_c< int, 4, 4, 4 > factors_t;
        typedef get_yuv_view< pixel_t, factors_t >::type view_t;

        
        view_t v = yuv_view< pixel_t
                           , factors_t
                           , boost::gil::IMC1
                           >( width
                            , height
                            , &data.front()
                            , width
                            );

        copy_pixels( color_converted_view< bgr8_pixel_t >( v ), view( image ));

        stringstream ss;
        ss << out_path + "test_yuv444__" << i << ".png";

        write_view( ss.str(), view( image ), png_tag() );
    }
}

BOOST_AUTO_TEST_CASE( image_read_test_yuv_420 )
{
    string filename = yuv_image_path + "videodump.raw";

    ifstream in( filename, std::fstream::binary );

    if( in == false )
    {
        return;
    }
    

    DumpHeader header;
    std::streamsize header_size = sizeof( DumpHeader );

    in.read( (char*) &header, header_size);

    std::size_t width  = header.xres;
    std::size_t height = header.yres;    

    std::streamsize y_plane_size = width * height;
    std::streamsize u_plane_size = ( width / 2 ) * ( height / 2 );
    std::streamsize v_plane_size = ( width / 2 ) * ( height / 2 );

    std::streamsize image_size = y_plane_size + u_plane_size + v_plane_size;

    vector< unsigned char > data( (unsigned int) image_size );

    rgb8_image_t image( width
                      , height
                      );

    for( int i = 0; i < 154; ++i )
    {
        in.read( (char*) &data.front(), image_size);

        typedef ycbcr_601_8_pixel_t pixel_t;
        typedef mpl::vector_c< int, 4, 2, 0 > factors_t;
        typedef get_yuv_view< pixel_t, factors_t >::type view_t;

        
        view_t v = yuv_view< pixel_t
                           , factors_t
                           , boost::gil::IMC2
                           >( width
                            , height
                            , &data.front()
                            , width
                            );

        copy_pixels( color_converted_view< bgr8_pixel_t >( v ), view( image ));

        stringstream ss;
        ss << out_path + "test_yuv420__" << i << ".png";

        write_view( ss.str(), view( image ), png_tag() );
    }
}

BOOST_AUTO_TEST_CASE( image_read_test_yuv__ )
{
    string filename = yuv_image_path + "tulips_yuv444_prog_planar_qcif.yuv";

    ifstream in( filename, std::fstream::binary );

    if( in == false )
    {
        return;
    }
    
    std::size_t x = 176;
    std::size_t y = 144;

    std::streamsize y_plane_size = x * y;
    std::streamsize u_plane_size = x * y;
    std::streamsize v_plane_size = x * y;

    std::streamsize image_size = y_plane_size + u_plane_size + v_plane_size;

    vector< unsigned char > data( (unsigned int) image_size );

    rgb8_image_t image( x, y );

    for( int i = 0; i < 6; ++i )
    {
        //in.read( (char*) &data.front(), image_size);

        //typedef unsigned char* channel_ptr_t;
        //typedef ycbcr_601__t color_space_t;
        ////typedef yuv_planar_pixel_iterator< channel_ptr_t, ycbcr_601__t > x_iterator;
        //typedef planar_pixel_iterator< channel_ptr_t, ycbcr_601__t > x_iterator;
        //typedef type_from_x_iterator< x_iterator >::view_t view_t;

        //channel_ptr_t y_base = &data.front();
        //channel_ptr_t v_base = y_base + y_plane_size;
        //channel_ptr_t u_base = v_base + x;


        //view_t v = planar_view< channel_ptr_t
        //                      , color_space_t
        //                      >( x
        //                       , y
        //                       , y_base
        //                       , u_base
        //                       , v_base
        //                       , x
        //                       );

        //copy_pixels( color_converted_view< bgr8_pixel_t >( v ), view( image ));

        //stringstream ss;
        //ss << "c:/chhenning/yuv/data/test_" << i << ".png";

        //write_view( ss.str(), view( image ), png_tag() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
