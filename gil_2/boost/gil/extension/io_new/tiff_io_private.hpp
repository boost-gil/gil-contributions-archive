/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_IO_PRIVATE_HPP
#define TIFF_IO_PRIVATE_HPP

/// \file
/// \brief  Support for reading and writing TIFF files
//
/// \author Christian Henning
///         

/// see http://www.remotesensing.org/libtiff/
/// or http://www.libtiff.org/man/


#include <fstream>
#include <algorithm>
#include <string>

#include <boost/static_assert.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/gil/extension/io_new/tiff_base.hpp>
#include <boost/gil/extension/io_new/tiff_io_private_write.hpp>

namespace boost { namespace gil { namespace detail {

template <typename Property>
bool get_property( const std::string& file_name
                 , typename Property::type& value
                 , tiff_tag                        )
{
   return get_property<Property>( tiff_open_for_read( file_name )
                                , value                           );

}

void read_image_info( tiff_file_t                 file
                    , basic_tiff_image_read_info& info )
{
   get_property<tiff_image_width>( file
                                 , info._width );

   get_property<tiff_image_height>( file
                                 , info._height );

   get_property<tiff_samples_per_pixel>( file
                                       , info._samples_per_pixel );

   get_property<tiff_bits_per_sample>( file
                                     , info._bits_per_sample );

   get_property<tiff_sample_format>( file
                                   , info._sample_format );

   get_property<tiff_planar_configuration>( file
                                          , info._planar_configuration );

   get_property<tiff_photometric_interpretation>( file
                                                , info._photometric_interpretation  );
}

template< typename SrcImageIsBitAligned
        , typename UserImageIsBitAligned
        , typename SrcImageIsPlanar
        , typename UserImageIsPlanar
        >
struct read_and_no_convert_impl
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {}

};

// Specialization for reading bit aligned images.
template<>
struct read_and_no_convert_impl< boost::mpl::bool_< true >
                               , boost::mpl::bool_< true >
                               , boost::mpl::bool_< false >
                               , boost::mpl::bool_< false > >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
      read_bit_aligned_data( v, top_left, file );
   }
};

// Specialization for reading interleaved images.
template<>
struct read_and_no_convert_impl< boost::mpl::bool_< false >
                               , boost::mpl::bool_< false >
                               , boost::mpl::bool_< false >
                               , boost::mpl::bool_< false > >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
      io_error_if( views_are_compatible< View_User
                                       , typename Image_TIFF::view_t >::value != true
                 , "Views are incompatible. You might want to use read_and_convert_image()" );

      read_data( v, top_left, 0, file );
   }
};

// Specialization for reading planar images.
template<>
struct read_and_no_convert_impl< boost::mpl::bool_< false >
                               , boost::mpl::bool_< false >
                               , boost::mpl::bool_< true >
                               , boost::mpl::bool_< true >
                               >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
      typedef typename Image_TIFF::view_t      view_tiff_t;
      typedef typename view_tiff_t::value_type pixel_tiff_t;

      io_error_if( views_are_compatible<View_User, view_tiff_t>::value != true
                 , "Views are incompatible. You might want to use read_and_convert_image()" );

      for( tsample_t sample = 0
         ; sample < num_channels< pixel_tiff_t >::value
         ; ++sample                                     )
      {
         typedef typename nth_channel_view_type< View_User >::type plane_t;
         plane_t plane = nth_channel_view( v, sample );
         read_data( plane, top_left, sample, file );
      }
   }
};

struct read_and_no_convert
{
   read_and_no_convert( tiff_file_t file )
   : _file( file )
   {}

   // To be called by when image type is void.
   template< typename Image_TIFF
           , typename View_User
           >
   void read( const View_User& v
            , const point_t&   top_left
            , boost::mpl::true_         ) {}

   // To be called by real image types.
   template< typename Image_TIFF
           , typename View_User
           >
   void read( const View_User& v
            , const point_t&   top_left
            , boost::mpl::false_         )
   {
      typedef typename Image_TIFF::view_t View_TIFF;

      typedef is_bit_aligned< View_TIFF >::type src_image_is_bit_aligned_t;
      typedef is_bit_aligned< View_User >::type user_image_is_bit_aligned_t;

      typedef is_planar< View_TIFF >::type src_image_is_planar_t;
      typedef is_planar< View_User >::type user_image_is_planar_t;

      read_and_no_convert_impl< src_image_is_bit_aligned_t
                              , user_image_is_bit_aligned_t
                              , src_image_is_planar_t
                              , user_image_is_planar_t
                              > impl;

      impl.operator()< Image_TIFF >( v, top_left, _file );
   }

protected:

   tiff_file_t _file;
};


template< typename IsBitAligned
        , typename IsPlanar
        >
struct read_and_convert_impl
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      ) {}
};

// Specialization for reading and converting bit aligned images.
template<>
struct read_and_convert_impl< boost::mpl::bool_< true >
                            , boost::mpl::bool_< false > >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
   }
};

// Specialization for reading and converting interleaved images.
template<>
struct read_and_convert_impl< boost::mpl::bool_< false >
                            , boost::mpl::bool_< false > >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
   }
};

// Specialization for reading and converting planar images.
template<>
struct read_and_convert_impl< boost::mpl::bool_< false >
                            , boost::mpl::bool_< true > >
{
   template< typename Image_TIFF
           , typename View_User
           >
   void operator() ( const View_User& v
                   , const point_t&   top_left
                   , tiff_file_t      file      )
   {
   }
};


template< typename Color_Converter >
struct read_and_convert
{
   read_and_convert( tiff_file_t file )
   : _file( file )
   {}

   void set_color_converter( const Color_Converter& cc )
   {
      _cc = cc;
   }

   // To be called by when image type is void.
   template< typename Image_TIFF
           , typename View_User
           >
   void read( const View_User& v
            , const point_t&   top_left
            , boost::mpl::true_         ) {}

   // To be called by real image types.
   template< typename Image_TIFF
           , typename View_User
           >
   void read( const View_User& v
            , const point_t&   top_left
            , boost::mpl::false_         )
   {
      typedef is_bit_aligned< View_User >::type is_bit_aligned_t;
      typedef is_planar     < View_User >::type is_planar_t;

      read_and_convert_impl< is_bit_aligned_t, is_planar_t> impl;
      impl.operator()< Image_TIFF >( v, top_left, _file );
   }

protected:

   tiff_file_t _file;

   Color_Converter _cc;
};

template< typename View >
struct read_palette_image
{
   read_palette_image( tiff_file_t file ) : _file( file ) {}

   template< typename Indices_View >
   void operator() ( const View&         src_view
                   , const Indices_View& indices_view ) {}

private:

   tiff_file_t _file;
};

template<>
struct read_palette_image< rgb16_view_t >
{
   read_palette_image< rgb16_view_t >( tiff_file_t file ) : _file( file ) {}

   template< typename Indices_View >
   void operator() ( const rgb16_view_t& src_view
                   , const Indices_View& indices_view )
                   
   {
      tiff_color_map::red   red;
      tiff_color_map::green green;
      tiff_color_map::blue  blue;

      TIFFGetFieldDefaulted( _file.get()
                           , tiff_color_map::tag
                           , &red
                           , &green
                           , &blue                 );

      typedef channel_traits< element_type< typename Indices_View::value_type >::type >::value_type channel_t;
      int num_colors = channel_traits< channel_t >::max_value();

      rgb16_planar_view_t palette = planar_rgb_view( num_colors
                                                   , 1
                                                   , red
                                                   , green
                                                   , blue
                                                   , sizeof( bits16 ) * num_colors );

      rgb16_planar_view_t::x_iterator palette_it = palette.row_begin( 0 );


      for( rgb16_view_t::y_coord_t y = 0; y < src_view.height(); ++y )
      {
         rgb16_view_t::x_iterator it  = src_view.row_begin( y );
         rgb16_view_t::x_iterator end = src_view.row_end( y );

         typename Indices_View::x_iterator indices_it = indices_view.row_begin( y );

         for( ; it != end; ++it, ++indices_it )
         {
            unsigned char i = at_c<0>( *indices_it );

            *it = *palette.xy_at( i, 0 );
         }
      }
   }

private:

   tiff_file_t _file;
};


template< typename Reader >
class tiff_reader : public Reader
{
public:

   tiff_reader( tiff_file_t file )
   : Reader( file )
   {
      read_image_info( file, _info );
   }

   tiff_reader( tiff_file_t                file
              , basic_tiff_image_read_info info )
   : Reader( file )
   , _info( info )
   {}

   template< typename Image >
   void read_image( Image& src_img )
   {
      src_img.recreate( _info._width
                      , _info._height );

      _top_left = point_t( 0, 0 );

      _apply( view( src_img ));
   }

   template< typename View >
   void read_view( const View&    src_view
                 , const point_t& top_left )
   {

      io_error_if( src_view.dimensions() != point_t( _info._width  - top_left.x
                                                   , _info._height - top_left.y )
                 , "User provided view has incorrect size."                       );
      
      _top_left = top_left;

      _apply( src_view );
   }


private:

   template< typename View >
   void _apply( const View& src_view )
   {
      if( _info._photometric_interpretation == PHOTOMETRIC_PALETTE )
      {
         // Steps:
         // 1. Read indices. It's an array of grayX_pixel_t.
         // 2. Read palette. It's an array of rgb16_pixel_t.
         // 3. ??? Create virtual image or transform the two arrays
         //    into a rgb16_image_t object. The latter might
         //    be a good first solution.

         read_palette_image< View > rpi( _file );

         unsigned int num_colors = 0;
         switch( _info._bits_per_sample )
         {
            case 1:
            {
               gray1_image_t indices( _info._width, _info._height );
               _read_samples_per_pixel( view( indices ));

               rpi( src_view
                  , view( indices ));

               break;
            }

            case 2:
            {
               gray2_image_t indices( _info._width, _info._height );
               _read_samples_per_pixel( view( indices ));

               rpi( src_view
                  , view( indices ));

               break;
            }

            case 4:
            {
               gray4_image_t indices( _info._width, _info._height );
               _read_samples_per_pixel( view( indices ));

               rpi( src_view
                  , view( indices ));

               break;
            }

            case 8:
            {
               gray8_image_t indices( _info._width, _info._height );
               _read_samples_per_pixel( view( indices ));

               rpi( src_view
                  , view( indices ));

               break;
            }

            case 16:
            {
               gray16_image_t indices( _info._width, _info._height );
               _read_samples_per_pixel( view( indices ));

               rpi( src_view
                  , view( indices ));

               break;
            }

            default:
            {
               io_error( "Not supported palette " );
            }
         }

         return;
      }

      _read_samples_per_pixel( src_view );
   }

   template< typename View >
   void _read_samples_per_pixel( const View& src_view )
   {
      switch( _info._samples_per_pixel )
      {
         case 1:
         {
            _read_bits_per_sample< gray_layout_t >( src_view );
            break;
         }

         case 3:
         {
            _read_bits_per_sample< rgb_layout_t >( src_view );
            break;
         }

         case 4:
         {
            _read_bits_per_sample< rgba_layout_t >( src_view );
            break;
         }

         default:
         {
            io_error( "Samples_Per_Pixel not supported." );
         }
      }
   }

   template< typename Layout
           , typename View
           >
   void _read_bits_per_sample( const View& src_view )
   {
      switch( _info._bits_per_sample )
      {
         case 1:
         {
            _read_sample_format< 1, Layout >( src_view );
            break;
         }

         case 4:
         {
            _read_sample_format< 4, Layout >( src_view );
            break;
         }

         case 8:
         {
            _read_sample_format< 8, Layout >( src_view );
            break;
         }

         case 16:
         {
            _read_sample_format< 16, Layout >( src_view );
            break;
         }

         case 32:
         {
            _read_sample_format< 32, Layout >( src_view );
            break;
         }

         case 64:
         {
            _read_sample_format< 64, Layout >( src_view );
            break;
         }

         default:
         {
            io_error( "Bits_Per_Sample not supported." );
         }
      }
   }

   template< int      BitsPerSample
           , typename Layout
           , typename View
           >
   void _read_sample_format( const View& src_view )
   {
      switch( _info._sample_format )
      {
         case SAMPLEFORMAT_UINT:
         {
            _read_planar< BitsPerSample, SAMPLEFORMAT_UINT, Layout >( src_view );
            break;
         }

         case SAMPLEFORMAT_INT:
         {
            _read_planar< BitsPerSample, SAMPLEFORMAT_INT, Layout >( src_view );
            break;
         }

         case SAMPLEFORMAT_IEEEFP:
         {
            _read_planar< BitsPerSample, SAMPLEFORMAT_IEEEFP, Layout >( src_view );
            break;
         }
         
         default:
         {
            io_error( "Sample format not supported." );
         }
      }
   }

   template< int      BitsPerSample
           , int      SampleFormat
           , typename Layout
           , typename View
           >
   void _read_planar( const View& src_view )
   {
      typedef channel_type_factory< BitsPerSample, SampleFormat >::type channel_t;
      typedef pixel_type_factory< channel_t, Layout >::type pixel_t;

      if( _info._planar_configuration == PLANARCONFIG_CONTIG )
      {
         typedef image_type_factory< false, pixel_t >::type image_t;

         read< image_t >( src_view
                        , _top_left
                        , is_void< image_t >::type() );

      }
      else if( _info._planar_configuration == PLANARCONFIG_SEPARATE )
      {
         typedef image_type_factory< true, pixel_t >::type image_t;

         read< image_t >( src_view
                        , _top_left
                        , is_void< image_t >::type() );

      }
      else
      {
         io_error( "Wrong planar configuration setting." );
      }
   }

private:

   basic_tiff_image_read_info _info;

   point_t _top_left;
};

} // detail
} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_HPP
