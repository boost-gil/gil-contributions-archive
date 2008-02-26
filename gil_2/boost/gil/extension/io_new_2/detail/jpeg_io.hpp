/*
  Copyright 2007-2008 Christian Henning, Andreas Pokorny
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_JPEG_IO_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_JPEG_IO_HPP_INCLUDED

#include <vector>
#include <boost/gil/extension/io_new_2/jpeg_tags.hpp>
#include <boost/gil/extension/io_new_2/detail/jpeg_supported_types.hpp>
//! \author Christian Henning and Andreas Pokorny

namespace boost { namespace gil { namespace detail {

template<typename Device>
struct jpeg_decompress_mgr
{
    jpeg_decompress_mgr( Device & file )
        : in(file)
    {
        _cinfo.err = jpeg_std_error( &_jerr );

        _src._jsrc.bytes_in_buffer = 0;
        _src._jsrc.next_input_byte = buffer;
        _src._jsrc.init_source = reinterpret_cast<void(*)(j_decompress_ptr)>(&jpeg_decompress_mgr<Device>::init_device);
        _src._jsrc.fill_input_buffer = reinterpret_cast<boolean(*)(j_decompress_ptr)>(&jpeg_decompress_mgr<Device>::fill_buffer);
        _src._jsrc.skip_input_data = reinterpret_cast<void(*)(j_decompress_ptr, long num_bytes)>(&jpeg_decompress_mgr<Device>::skip_input_data);
        _src._jsrc.resync_to_restart= jpeg_resync_to_restart;
        _src._jsrc.term_source= reinterpret_cast<void(*)(j_decompress_ptr)>(&jpeg_decompress_mgr<Device>::close_device);
        _src._this = this;


        jpeg_create_decompress( &_cinfo );

        _cinfo.src = &_src._jsrc;

        jpeg_read_header( &_cinfo
                        , TRUE    );

        io_error_if( _cinfo.data_precision != 8, "Image file is not supported." );
    }

    ~jpeg_decompress_mgr()
    {
        jpeg_destroy_decompress( &_cinfo );
    }

    const jpeg_decompress_struct& get() const
    {
        return _cinfo;
    }

    jpeg_decompress_struct& get()
    {
        return _cinfo;
    }

protected:

    void start_decompress()
    {
        if( jpeg_start_decompress( &_cinfo ) == false )
        {
            io_error( "Cannot start decompression." );
        }
    }

    void finish_decompress()
    {
        jpeg_finish_decompress ( &_cinfo );
    }

private:

    static void init_device( jpeg_decompress_struct * cinfo )
    {
        gil_jpeg_source_mgr * src = reinterpret_cast<gil_jpeg_source_mgr*>(cinfo->src);
        src->_jsrc.bytes_in_buffer = 0;
        src->_jsrc.next_input_byte = src->_this->buffer;
    }
    static boolean fill_buffer( jpeg_decompress_struct * cinfo )
    {
        gil_jpeg_source_mgr * src = reinterpret_cast<gil_jpeg_source_mgr*>(cinfo->src);
        size_t count= src->_this->in.read(src->_this->buffer, sizeof(src->_this->buffer) );
        if( count <= 0 ) 
        {
            // libjpeg does that: adding an EOF marker 
            src->_this->buffer[0] = (JOCTET) 0xFF;
            src->_this->buffer[1] = (JOCTET) JPEG_EOI;
            count = 2;
        }

        src->_jsrc.next_input_byte = src->_this->buffer;
        src->_jsrc.bytes_in_buffer = count;

        return TRUE;
    }
    static void skip_input_data( jpeg_decompress_struct * cinfo, long num_bytes  )
    {
        gil_jpeg_source_mgr * src = reinterpret_cast<gil_jpeg_source_mgr*>(cinfo->src);

        if (num_bytes > 0) 
        {
            if( num_bytes > long(src->_jsrc.bytes_in_buffer) )
            {
                src->_jsrc.bytes_in_buffer = 0;
                src->_this->in.seek( num_bytes, SEEK_CUR);
                num_bytes = 0;
            }
            src->_jsrc.next_input_byte += num_bytes;
            src->_jsrc.bytes_in_buffer -= num_bytes;
        }
    }

    static void close_device( jpeg_decompress_struct * cinfo )
    {
    }

protected:
    jpeg_decompress_struct _cinfo;
    jpeg_error_mgr         _jerr;
private:
    Device &in;
    struct gil_jpeg_source_mgr{
        jpeg_source_mgr _jsrc;
        jpeg_decompress_mgr * _this;
    };
    gil_jpeg_source_mgr _src;
    JOCTET buffer[1024];
};

template< typename Device
        , typename ConversionPolicy
        >
struct reader<Device,jpeg_tag,ConversionPolicy> 
    : jpeg_decompress_mgr<Device>
{
public:
    reader( Device & device )
        : jpeg_decompress_mgr<Device>(device)
    {}

    reader( Device & device, typename ConversionPolicy::color_converter_type const& cc )
        : jpeg_decompress_mgr<Device>(device), cc_policy(cc)
    {
    }

    image_read_info<jpeg_tag> get_info()
    {
        image_read_info<jpeg_tag> ret;
        ret._width          = this->_cinfo.image_width;
        ret._height         = this->_cinfo.image_height;
        ret._num_components = this->_cinfo.num_components;
        ret._color_space    = this->_cinfo.jpeg_color_space;
        ret._data_precision = this->_cinfo.data_precision;

        return ret;
    }

    template<typename Image>
    void read_image( Image&         image
                   , const point_t& top_left
                   , const point_t& bottom_right
                   )
    {
        image_read_info<jpeg_tag> info(get_info());

        check_coordinates( top_left
                         , bottom_right
                         , info
                         );

        _top_left = top_left;

        if( bottom_right == point_t( 0, 0 ))
        {
            _bottom_right.x = info._width  - 1;
            _bottom_right.y = info._height - 1;
        }
        else
        {
            _bottom_right = bottom_right;
        }

        image.recreate( ( _bottom_right.x + 1 ) - top_left.x
                      , ( _bottom_right.y + 1 ) - top_left.y );

        apply_impl( view( image )
                  , info
                  );
    }

    template<typename View>
    void read_view( const View&    view
                  , const point_t& top_left
                  , const point_t& bottom_right
                  )
    {
        image_read_info<jpeg_tag> info(get_info());

        check_coordinates( top_left
                         , bottom_right
                         , info
                         );

        _top_left = top_left;

        if( bottom_right == point_t( 0, 0 ))
        {
            _bottom_right.x = info._width  - 1;
            _bottom_right.y = info._height - 1;
        }
        else
        {
            _bottom_right = bottom_right;
        }

        apply_impl( view
                  , info
                  );
    }
private:

    template<typename View>
    void apply_impl( const View& view
                   , const image_read_info<jpeg_tag>& info )
    {
        start_decompress();

        switch( info._color_space )
        {
        case JCS_GRAYSCALE:
            io_error_if(info._num_components!=1,"reader<jpeg>: error in image data");
            read_rows<gray8_pixel_t>( view );
            break;
        case JCS_RGB:
            io_error_if(info._num_components!=3,"reader<jpeg>: error in image data");
            read_rows<rgb8_pixel_t>( view );
        case JCS_YCbCr:
            io_error_if(info._num_components!=3,"reader<jpeg>: error in image data");
            //!\todo add Y'CbCr? We loose image quality when reading JCS_YCbCr as JCS_RGB
            this->_cinfo.out_color_space = JCS_RGB;
            read_rows<rgb8_pixel_t>( view );
            break;
        case JCS_CMYK:
            io_error_if(info._num_components!=4,"reader<jpeg>: error in image data");
            read_rows<cmyk8_pixel_t>( view );
            break;
        case JCS_YCCK:
            io_error_if(info._num_components!=4,"reader<jpeg>: error in image data");
            //!\todo add Y'CbCrK? We loose image quality when reading JCS_YCCK as JCS_CMYK
            this->_cinfo.out_color_space = JCS_CMYK;
            read_rows<cmyk8_pixel_t>( view );
            break;
        default:
            break;
            // unknown
        }

        finish_decompress();
    }

    template< typename ImagePixel
            , typename View
            >
    void read_rows( const View& view )
    {
        io_error_if( ! ConversionPolicy::template is_allowed< ImagePixel
                                                            , typename View::value_type
                                                            >::type::value
                   , "User provided view has incorrect color space or channel type."
                   );

        std::vector<ImagePixel> buffer( view.width() );

        JSAMPLE *row_adr = reinterpret_cast< JSAMPLE* >( &buffer[0] );

        for( int y=0; y < view.height(); ++y )
        {
            io_error_if( jpeg_read_scanlines( &this->_cinfo
                                            , &row_adr
                                            , 1
                                            ) !=1
                       , "jpeg_read_scanlines: fail to read JPEG file" );

            cc_policy.read( buffer.begin() + _top_left.x
                          , buffer.begin() + _bottom_right.x + 1
                          , view.row_begin( y )
                          );
        }
    }

    point_t _top_left;
    point_t _bottom_right;

    ConversionPolicy cc_policy;
};

template< typename Device >
struct writer<Device,jpeg_tag> 
{
    writer( Device & file )
        : out(file)
    {
        _dest._jdest.free_in_buffer = sizeof(buffer);
        _dest._jdest.next_output_byte = buffer;
        _dest._jdest.init_destination = reinterpret_cast<void(*)(j_compress_ptr)>(&writer<Device,jpeg_tag>::init_device);
        _dest._jdest.empty_output_buffer = reinterpret_cast<boolean(*)(j_compress_ptr)>(&writer<Device,jpeg_tag>::empty_buffer);
        _dest._jdest.term_destination = reinterpret_cast<void(*)(j_compress_ptr)>(&writer<Device,jpeg_tag>::close_device);
        _dest._this = this;

        _cinfo.err = jpeg_std_error( &_jerr );

        jpeg_create_compress( &_cinfo  );
        _cinfo.dest = &_dest._jdest;

    }

    ~writer()
    {
        jpeg_finish_compress ( &_cinfo );
        jpeg_destroy_compress( &_cinfo );
    }

    jpeg_compress_struct& get()
    {
        return _cinfo;
    }


    template<typename View>
    void apply( const View& view )
    {
        _cinfo.image_width  = JDIMENSION(view.width());
        _cinfo.image_height = JDIMENSION(view.height());
        _cinfo.input_components = num_channels<View>::value;
        _cinfo.in_color_space = detail::jpeg_rw_support<typename channel_type<View>::type,
            typename color_space_type<View>::type>::color_type;
        jpeg_set_defaults(&_cinfo);
        jpeg_set_quality(&_cinfo, 100, TRUE);

        write_rows( view );
    }

    template<typename View>
    void apply( const View&                       view
              , const image_write_info<jpeg_tag>& info )
    {
        _cinfo.image_width  = JDIMENSION(view.width());
        _cinfo.image_height = JDIMENSION(view.height());
        _cinfo.input_components = num_channels<View>::value;
        _cinfo.in_color_space = detail::jpeg_rw_support< typename channel_type<View>::type
                                                       , typename color_space_type<View>::type
                                                       >::color_type;

        jpeg_set_defaults( &_cinfo);
        jpeg_set_quality ( &_cinfo
                         , info._quality
                         , TRUE
                         );

        write_rows( view );
    }

private:

    template<typename View>
    void write_rows( const View& view )
    {
        jpeg_start_compress(&_cinfo, TRUE);

        std::vector<typename View::value_type> row_buffer( view.width() );
        JSAMPLE* row_addr = reinterpret_cast<JSAMPLE*>( &row_buffer[0] );

        for( int y =0; y != view.height(); ++ y )
        {
            std::copy( view.row_begin( y )
                     , view.row_end  ( y )
                     , row_buffer.begin()
                     );

            jpeg_write_scanlines( &this->_cinfo
                                , &row_addr
                                , 1
                                );
        }
    }

    struct gil_jpeg_destination_mgr
    {
        jpeg_destination_mgr _jdest;
        writer<Device,jpeg_tag> * _this;
    };

    static void init_device( jpeg_compress_struct * cinfo )
    {
        gil_jpeg_destination_mgr * dest = reinterpret_cast<gil_jpeg_destination_mgr*>(cinfo->dest);
        dest->_jdest.free_in_buffer = sizeof(dest->_this->buffer);
        dest->_jdest.next_output_byte = dest->_this->buffer;
    }
    static boolean empty_buffer( jpeg_compress_struct * cinfo )
    {
        gil_jpeg_destination_mgr * dest = reinterpret_cast<gil_jpeg_destination_mgr*>(cinfo->dest);
        dest->_this->out.write(
                reinterpret_cast<unsigned char*>(dest->_this->buffer), 
                ( dest->_jdest.next_output_byte - dest->_this->buffer ) / sizeof(JOCTET) 
                );


        writer<Device,jpeg_tag>::init_device( cinfo );
        return 1;
    }
    static void close_device( jpeg_compress_struct * cinfo )
    {
        writer<Device,jpeg_tag>::empty_buffer( cinfo );

        gil_jpeg_destination_mgr * dest = 
            reinterpret_cast<gil_jpeg_destination_mgr*>(cinfo->dest);

        dest->_this->out.flush();
    }

    Device &out;

    jpeg_compress_struct _cinfo;
    jpeg_error_mgr       _jerr;

    gil_jpeg_destination_mgr _dest;
    JOCTET buffer[1024];
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_JPEG_IO_HPP_INCLUDED