/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/extension/io_new/png_tags.hpp>

#include <boost/gil/gil_all.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>

#include "base.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil { 

///
/// PNG Backend
///
template<typename Device >
struct reader_backend< Device
                     , png_tag
                     >
    : public png_io_base< Device >
{
    reader_backend( const Device&                         device
                  , const image_read_settings< png_tag >& settings
                  )
    : png_io_base< Device >( device )

    , _settings( settings )
    , _info()

    , _png_ptr ( NULL )
    , _info_ptr( NULL )
    , _number_passes( 0 )
    {}

    image_read_settings< png_tag > _settings;
    image_read_info    < png_tag > _info;

    png_structp _png_ptr;
    png_infop   _info_ptr;

    std::size_t _number_passes;
};

///
/// PNG Reader
///
template< typename Device >
class scanline_reader< Device
                     , png_tag
                     >
    : public reader_backend< Device
                           , png_tag
                           >
{
private:

    typedef scanline_reader< Device
                           , png_tag
                           > this_t;

public:

    typedef reader_backend< Device, png_tag > backend_t;

public:

    //
    // Constructor
    //
    scanline_reader( const Device&                         io_dev
                   , const image_read_settings< png_tag >& settings
                   )
    : reader_backend< Device
                    , png_tag
                    >( io_dev
                     , settings
                     )
    {}

    //
    // Destructor
    //
    ~scanline_reader()
    {
        png_destroy_read_struct( &_png_ptr
                               , &_info_ptr
                               , NULL
                               );
    }


    void read_header()
    {
        // check the file's first few bytes
        byte_t buf[PNG_BYTES_TO_CHECK];

        io_error_if( _io_dev.read( buf
                                , PNG_BYTES_TO_CHECK
                                ) != PNG_BYTES_TO_CHECK
                   , "png_check_validity: failed to read image"
                   );

        io_error_if( png_sig_cmp( png_bytep(buf)
                                , png_size_t(0)
                                , PNG_BYTES_TO_CHECK
                                ) != 0
                   , "png_check_validity: invalid png image"
                   );

        // Create and initialize the png_struct with the desired error handler
        // functions.  If you want to use the default stderr and longjump method,
        // you can supply NULL for the last three parameters.  We also supply the
        // the compiler header file version, so that we know if the application
        // was compiled with a compatible version of the library.  REQUIRED
        _png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING
                                         , NULL  // user_error_ptr
                                         , NULL  // user_error_fn
                                         , NULL  // user_warning_fn
                                         );

        io_error_if( _png_ptr == NULL
                   , "png_reader: fail to call png_create_write_struct()"
                   );

        png_uint_32 user_chunk_data[4];
        user_chunk_data[0] = 0;
        user_chunk_data[1] = 0;
        user_chunk_data[2] = 0;
        user_chunk_data[3] = 0;
        png_set_read_user_chunk_fn( _png_ptr
                                  , user_chunk_data
                                  , png_io_base< Device >::read_user_chunk_callback
                                  );

        // Allocate/initialize the memory for image information.  REQUIRED.
        _info_ptr = png_create_info_struct( _png_ptr );

        if( _info_ptr == NULL )
        {
            png_destroy_read_struct( &_png_ptr
                                   , NULL
                                   , NULL
                                   );

            io_error( "png_reader: fail to call png_create_info_struct()" );
        }

        // Set error handling if you are using the setjmp/longjmp method (this is
        // the normal method of doing things with libpng).  REQUIRED unless you
        // set up your own error handlers in the png_create_read_struct() earlier.
        if( setjmp( png_jmpbuf( _png_ptr )))
        {
            //free all of the memory associated with the png_ptr and info_ptr
            png_destroy_read_struct( &_png_ptr
                                   , &_info_ptr
                                   , NULL
                                   );

            io_error( "png is invalid" );
        }

        png_set_read_fn( _png_ptr
                       , static_cast< png_voidp >( &this->_io_dev )
                       , png_io_base< Device >::read_data
                       );

        // Set up a callback function that will be
        // called after each row has been read, which you can use to control
        // a progress meter or the like.
        png_set_read_status_fn( _png_ptr
                              , png_io_base< Device >::read_row_callback
                              );

        // Set up a callback which implements user defined transformation.
        // @todo
        png_set_read_user_transform_fn( _png_ptr
                                      , png_user_transform_ptr( NULL )
                                      );

        png_set_keep_unknown_chunks( _png_ptr
                                   , PNG_HANDLE_CHUNK_ALWAYS
                                   , NULL
                                   , 0
                                   );


        // Make sure we read the signature.
        // @todo make it an option
        png_set_sig_bytes( _png_ptr
                         , PNG_BYTES_TO_CHECK
                         );

        // The call to png_read_info() gives us all of the information from the
        // PNG file before the first IDAT (image data chunk).  REQUIRED
        png_read_info( _png_ptr
                     , _info_ptr
                     );

        ///
        /// Start reading the image information
        ///

        // get PNG_IHDR chunk information from png_info structure
        png_get_IHDR( _png_ptr
                    , _info_ptr
                    , &this->_info._width
                    , &this->_info._height
                    , &this->_info._bit_depth
                    , &this->_info._color_type
                    , &this->_info._interlace_method
                    , &this->_info._compression_method
                    , &this->_info._filter_method
                    );

        // get number of color channels in image
        this->_info._num_channels = png_get_channels( _png_ptr
                                              , _info_ptr
                                              );

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

        // Get CIE chromacities and referenced white point
        if( this->_settings._read_cie_chromacities )
        {
            this->_info._valid_cie_colors = png_get_cHRM( _png_ptr
                                                        , _info_ptr
                                                        , &this->_info._white_x, &this->_info._white_y
                                                        ,   &this->_info._red_x,   &this->_info._red_y
                                                        , &this->_info._green_x, &this->_info._green_y
                                                        ,  &this->_info._blue_x,  &this->_info._blue_y
                                                        );
        }

        // get the gamma value
        if( this->_settings._read_file_gamma )
        {
            this->_info._valid_file_gamma = png_get_gAMA( _png_ptr
                                                        , _info_ptr
                                                        , &this->_info._file_gamma
                                                        );

            if( this->_info._valid_file_gamma == false )
            {
                this->_info._file_gamma = 1.0;
            }
        }
#else

        // Get CIE chromacities and referenced white point
        if( this->_settings._read_cie_chromacities )
        {
            this->_info._valid_cie_colors = png_get_cHRM_fixed( _png_ptr
                                                              , _info_ptr
                                                              , &this->_info._white_x, &this->_info._white_y
                                                              ,   &this->_info._red_x,   &this->_info._red_y
                                                              , &this->_info._green_x, &this->_info._green_y
                                                              ,  &this->_info._blue_x,  &this->_info._blue_y
                                                              );
        }

        // get the gamma value
        if( this->_settings._read_file_gamma )
        {
            this->_info._valid_file_gamma = png_get_gAMA_fixed( _png_ptr
                                                              , _info_ptr
                                                              , &this->_info._file_gamma
                                                              );

            if( this->_info._valid_file_gamma == false )
            {
                this->_info._file_gamma = 1;
            }
        }
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

        // get the embedded ICC profile data
        if( this->_settings._read_icc_profile )
        {
#if PNG_LIBPNG_VER_MINOR >= 5
            png_charp icc_name = png_charp( NULL );
            png_bytep profile  = png_bytep( NULL );

            this->_info._valid_icc_profile = png_get_iCCP( _png_ptr
                                                         , _info_ptr
                                                         , &icc_name
                                                         , &this->_info._iccp_compression_type
                                                         , &profile
                                                         , &this->_info._profile_length
                                                         );
#else
            png_charp icc_name = png_charp( NULL );
            png_charp profile  = png_charp( NULL );

            this->_info._valid_icc_profile = png_get_iCCP( _png_ptr
                                                         , _info_ptr
                                                         , &icc_name
                                                         , &this->_info._iccp_compression_type
                                                         , &profile
                                                         , &this->_info._profile_length
                                                         );
#endif
            if( icc_name )
            {
                this->_info._icc_name.append( icc_name
                                            , std::strlen( icc_name )
                                            );
            }

            if( this->_info._profile_length > 0 )
            {
                this->_info._profile.append( reinterpret_cast< char* >( profile )
                                           , this->_info._profile_length
                                           );
            }
        }

        // get the rendering intent
        if( this->_settings._read_intent )
        {
            this->_info._valid_intent = png_get_sRGB( _png_ptr
                                                    , _info_ptr
                                                    , &this->_info._intent
                                                    );
        }

        // get image palette information from png_info structure
        if( this->_settings._read_palette )
        {
            png_colorp palette = png_colorp( NULL );

            this->_info._valid_palette = png_get_PLTE( _png_ptr
                                                     , _info_ptr
                                                     , &palette
                                                     , &this->_info._num_palette
                                                     );

            if( this->_info._num_palette > 0 )
            {
                this->_info._palette.resize( this->_info._num_palette );
                std::copy( palette
                         , palette + this->_info._num_palette
                         , &this->_info._palette.front()
                         );
            }
        }

        // get background color
        if( this->_settings._read_background )
        {
            png_color_16p background = png_color_16p( NULL );

            this->_info._valid_background = png_get_bKGD( _png_ptr
                                                        , _info_ptr
                                                        , &background
                                                        );
            if( background )
            {
                this->_info._background = *background;
            }
        }

        // get the histogram
        if( this->_settings._read_histogram )
        {
            png_uint_16p histogram = png_uint_16p( NULL );

            this->_info._valid_histogram = png_get_hIST( _png_ptr
                                                       , _info_ptr
                                                       , &histogram
                                                       );

            if( histogram )
            {
                // the number of values is set by the number of colors inside
                // the palette.
                if( this->_settings._read_palette == false )
                {
                    png_colorp palette = png_colorp( NULL );
                    png_get_PLTE( _png_ptr
                                , _info_ptr
                                , &palette
                                , &this->_info._num_palette
                                );
                }

                std::copy( histogram
                         , histogram + this->_info._num_palette
                         , &this->_info._histogram.front()
                         );
            }
        }

        // get screen offsets for the given image
        if( this->_settings._read_screen_offsets )
        {
            this->_info._valid_offset = png_get_oFFs( _png_ptr
                                                    , _info_ptr
                                                    , &this->_info._offset_x
                                                    , &this->_info._offset_y
                                                    , &this->_info._off_unit_type
                                                    );
        }


        // get pixel calibration settings
        if( this->_settings._read_pixel_calibration )
        {
            png_charp purpose = png_charp ( NULL );
            png_charp units   = png_charp ( NULL );
            png_charpp params = png_charpp( NULL );

            this->_info._valid_pixel_calibration = png_get_pCAL( _png_ptr
                                                               , _info_ptr
                                                               , &purpose
                                                               , &this->_info._X0
                                                               , &this->_info._X1
                                                               , &this->_info._cal_type
                                                               , &this->_info._num_params
                                                               , &units
                                                               , &params
                                                               );
            if( purpose )
            {
                this->_info._purpose.append( purpose
                                           , std::strlen( purpose )
                                           );
            }

            if( units )
            {
                this->_info._units.append( units
                                         , std::strlen( units )
                                         );
            }

            if( this->_info._num_params > 0 )
            {
                this->_info._params.resize( this->_info._num_params );

                for( png_CAL_nparam::type i = 0
                   ; i < this->_info._num_params
                   ; ++i
                   )
                {
                    this->_info._params[i].append( params[i]
                                                 , std::strlen( params[i] )
                                                 );
                }
            }
        }

        // get the physical resolution
        if( this->_settings._read_physical_resolution )
        {
            this->_info._valid_resolution = png_get_pHYs( _png_ptr
                                                        , _info_ptr
                                                        , &this->_info._res_x
                                                        , &this->_info._res_y
                                                        , &this->_info._phy_unit_type
                                                        );
        }

        // get the image resolution in pixels per meter.
        if( this->_settings._read_pixels_per_meter )
        {
            this->_info._pixels_per_meter = png_get_pixels_per_meter( _png_ptr
                                                                    , _info_ptr
                                                                    );
        }


        // get number of significant bits for each color channel
        if( this->_settings._read_number_of_significant_bits )
        {
            png_color_8p sig_bits = png_color_8p( NULL );

            this->_info._valid_significant_bits = png_get_sBIT( _png_ptr
                                                              , _info_ptr
                                                              , &sig_bits
                                                              );

            // @todo Is there one or more colors?
            if( sig_bits )
            {
                this->_info._sig_bits = *sig_bits;
            }
        }

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

        // get physical scale settings
        if( this->_settings._read_scale_factors )
        {
            this->_info._valid_scale_factors = png_get_sCAL( _png_ptr
                                                           , _info_ptr
                                                           , &this->_info._scale_unit
                                                           , &this->_info._scale_width
                                                           , &this->_info._scale_height
                                                           );
        }
#else
#ifdef BOOST_GIL_IO_PNG_FIXED_POINT_SUPPORTED
        if( this->_settings._read_scale_factors )
        {
            png_charp scale_width, scale_height;

            if( this->_info._valid_scale_factors = png_get_sCAL_s( _png_ptr
                                                                 , _info_ptr
                                                                 , &this->_info._scale_unit
                                                                 , &scale_width
                                                                 , &scale_height
                                                                 )
              )
            {
                if( scale_width )
                {
                    this->_info._scale_width.append( scale_width
                                                   , std::strlen( scale_width )
                                                   );
                }

                if( scale_height )
                {
                    this->_info._scale_height.append( scale_height
                                                    , std::strlen( scale_height )
                                                    );
                }
            }
        }
#endif // BOOST_GIL_IO_PNG_FIXED_POINT_SUPPORTED
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED

        // get comments information from png_info structure
        if( this->_settings._read_comments )
        {
            png_textp text = png_textp( NULL );

            this->_info._valid_text = png_get_text( _png_ptr
                                                  , _info_ptr
                                                  , &text
                                                  , &this->_info._num_text
                                                  );

            if( this->_info._num_text > 0 )
            {
                this->_info._text.resize( this->_info._num_text );

                for( png_num_text::type i = 0
                   ; i < this->_info._num_text
                   ; ++i
                   )
                {
                    this->_info._text[i]._compression = text[i].compression;
                    this->_info._text[i]._key.append( text[i].key
                                                    , std::strlen( text[i].key )
                                                    );

                    this->_info._text[i]._text.append( text[i].text
                                                     , std::strlen( text[i].text )
                                                     );
                }
            }
        }

        // get last modification time
        if( this->_settings._read_last_modification_time )
        {
            png_timep mod_time = png_timep( NULL );
            this->_info._valid_modification_time = png_get_tIME( _png_ptr
                                                               , _info_ptr
                                                               , &mod_time
                                                               );
            if( mod_time )
            {
                this->_info._mod_time = *mod_time;
            }
        }

        // get transparency data
        if( this->_settings._read_transparency_data )
        {
            png_bytep     trans        = png_bytep    ( NULL );
            png_color_16p trans_values = png_color_16p( NULL );

            this->_info._valid_transparency_factors = png_get_tRNS( _png_ptr
                                                                  , _info_ptr
                                                                  , &trans
                                                                  , &this->_info._num_trans
                                                                  , &trans_values
                                                                  );

            if( trans )
            {
                //@todo What to do, here? How do I know the length of the "trans" array?
            }

            if( this->_info._num_trans )
            {
                this->_info._trans_values.resize( this->_info._num_trans );
                std::copy( trans_values
                         , trans_values + this->_info._num_trans
                         , &this->_info._trans_values.front()
                         );
            }
        }

        // @todo One day!
/*
        if( false )
        {
            png_unknown_chunkp unknowns = png_unknown_chunkp( NULL );
            int num_unknowns = static_cast< int >( png_get_unknown_chunks( _png_ptr
                                                                         , _info_ptr
                                                                         , &unknowns
                                                                         )
                                                 );
        }
*/
    }

    void initialize()
    {
        // The info structures are filled at this point.

        // Now it's time for some transformations.

        if( little_endian() )
        {
            if( this->_info._bit_depth == 16 )
            {
                // Swap bytes of 16 bit files to least significant byte first.
                png_set_swap( _png_ptr );
            }

            if( this->_info._bit_depth < 8 )
            {
                // swap bits of 1, 2, 4 bit packed pixel formats
                png_set_packswap( _png_ptr );
            }
        }

        if( this->_info._color_type == PNG_COLOR_TYPE_PALETTE )
        {
            png_set_palette_to_rgb( _png_ptr );
        }

        if( this->_info._num_trans > 0 )
        {
            png_set_tRNS_to_alpha( _png_ptr );
        }

        // Tell libpng to handle the gamma conversion for you.  The final call
        // is a good guess for PC generated images, but it should be configurable
        // by the user at run time by the user.  It is strongly suggested that
        // your application support gamma correction.
        if( this->_settings._apply_screen_gamma )
        {
            // png_set_gamma will change the image data!

#ifdef BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        png_set_gamma( _png_ptr
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#else
        png_set_gamma( _png_ptr
                     , this->_settings._screen_gamma
                     , this->_info._file_gamma
                     );
#endif // BOOST_GIL_IO_PNG_FLOATING_POINT_SUPPORTED
        }

        // Turn on interlace handling.  REQUIRED if you are not using
        // png_read_image().  To see how to handle interlacing passes,
        // see the png_read_row() method below:
        _number_passes = png_set_interlace_handling( _png_ptr );
        io_error_if( _number_passes != 1, "scanline_read_iterator cannot read interlaced png images." );


        // The above transformation might have changed the bit_depth and color type.
        png_read_update_info( _png_ptr
                            , _info_ptr
                            );

        this->_info._bit_depth = png_get_bit_depth( _png_ptr
                                                  , _info_ptr
                                                  );

        this->_info._num_channels = png_get_channels( _png_ptr
                                                    , _info_ptr
                                                    );

        this->_info._color_type = png_get_color_type( _png_ptr
                                                    , _info_ptr
                                                    );
    }

    void clean_up()
    {
        // read rest of file, and get additional chunks in info_ptr

        ///@todo
        //png_read_end( _png_ptr
        //            , NULL
        //            );
    }

    /// Return length of scanline in bytes.
    std::size_t scanline_length()
    {
        return png_get_rowbytes( _png_ptr
                               , _info_ptr
                               );
    }

    void read( byte_t* dst, int pos )
    {
        read_scanline(dst);
    }

private:

    void read_scanline( byte_t* dst )
    {
        png_read_rows( _png_ptr
                     , &dst
                     , NULL
                     , 1
                     );
    }


private:

    boost::function< void ( this_t*, byte_t* ) > _read_function;
};

struct png_type_format_checker
{
    png_type_format_checker( png_bitdepth::type   bit_depth
                           , png_color_type::type color_type
                           )
    : _bit_depth ( bit_depth  )
    , _color_type( color_type )
    {}

    template< typename Image >
    bool apply()
    {
        typedef is_read_supported< typename get_pixel_type< typename Image::view_t >::type
                                 , png_tag
                                 > is_supported_t;

        return is_supported_t::_bit_depth  == _bit_depth
            && is_supported_t::_color_type == _color_type;
    }

private:

    png_bitdepth::type   _bit_depth;
    png_color_type::type _color_type;
};

struct png_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , png_tag
                                           >
    {};
};

template< typename Device
        , typename View
        >
class dynamic_image_reader< Device
                          , png_tag
                          , View
                          >
    : public reader< Device
                   , png_tag
                   , read_and_no_convert
                   , View
                   >
{
    typedef reader< Device
                  , png_tag
                  , read_and_no_convert
                  , View
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< png_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        png_type_format_checker format_checker( this->_info._bit_depth
                                              , this->_info._color_type
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

            detail::dynamic_io_fnobj< png_read_is_supported
                                    , parent_t
                                    > op( this );

            apply_operation( view( images )
                           , op
                           );
        }
    }
};

} // namespace gil
} // namespace boost



#endif // BOOST_GIL_EXTENSION_IO_DETAIL_PNG_IO_READ_HPP
