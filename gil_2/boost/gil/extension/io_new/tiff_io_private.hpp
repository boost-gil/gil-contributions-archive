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

namespace boost { namespace gil { 

namespace detail {

template< typename String >
tiff_file_t tiff_open_for_read( const String& file_name )
{
   BOOST_STATIC_ASSERT( false );
}

template<>
tiff_file_t tiff_open_for_read<std::string>( const std::string& file_name )
{
   TIFF* tiff;
   io_error_if((tiff = TIFFOpen( file_name.c_str(), "r" ) ) == NULL, "file_mgr: failed to open file" );

   return tiff_file_t( tiff, TIFFClose );
}

//shared_ptr<TIFF> tiff_open_for_write(file_name);

template <typename Property>
bool get_property( tiff_file_t              file
                 , typename Property::type& value
                 , tiff_tag                        )
{
   if( TIFFGetFieldDefaulted( file.get(), Property::tag, &value ) == 1 )
   {
      return true;
   }

   return false;
}

template <typename Property>
bool get_property( const std::string& file_name
                 , typename Property::type& value
                 , tiff_tag                        )
{
   return get_property<Property>( tiff_open_for_read( file_name )
                                , value
                                , tiff_tag()                        );

}

void read_image_info( tiff_file_t                 file
                    , basic_tiff_image_read_info& info )
{
   get_property<tiff_image_width>( file
                                 , info._width
                                 , tiff_tag()   );

   get_property<tiff_image_height>( file
                                 , info._height
                                 , tiff_tag()   );

   get_property<tiff_samples_per_pixel>( file
                                       , info._samples_per_pixel
                                       , tiff_tag()              );

   get_property<tiff_bits_per_sample>( file
                                     , info._bits_per_sample
                                     , tiff_tag()              );

   get_property<tiff_sample_format>( file
                                   , info._sample_format
                                   , tiff_tag()          );

   get_property<tiff_planar_configuration>( file
                                          , info._planar_configuration
                                          , tiff_tag()                );
}

class tiff_reader
{
public:

   tiff_reader( tiff_file_t file )
   : _tiff( file )
   {
      read_image_info( file, _info );
   }

   template< typename Image >
   void read( Image& img )
   {
      img.recreate( _info._width, _info._height );

      _read( view( img ));
   }

private:

   /*
      This goes back to consistency with GIL.
      GIL allows standard binary operations (assignment, equality comparison,
      copy construction) only between compatible constructs. They must have
      the same color space and pairwise compatible channels. Thus standard
      operations are lossless.

      We don't want to have a silent lossy operation, similar to the implicit
      conversion in C++ - this is a source of numerous hard to find bugs. Alex
      Stepanov, in fact, thinks the standard binary operations should only be
      defined when both arguments are the same type, but in GIL we relax this
      requirement slightly, as long as the types are compatible.

      Then there is a set of operations that explicitly converts one type to
      the other, such as copy_and_convert_pixels. These are lossy and the way
      to convert from one to the other is specified by a color conversion
      object. (A default is provided if left unspecified)

      Since we can think of the image on file as an elaborate virtual image,
      we need to provide the equivalent read_and_convert operation (actually
      two, one taking an optional color conversion object and the other using
      the default).
      Also, the read operations (with no conversion) should throw an exception
      if the image on file is not compatible.   
   */

   template< typename View >
   void _read( const View& v )
   {
      if( _info._planar_configuration == PLANARCONFIG_CONTIG )
      {
         // interleaved images

         switch(  _info._samples_per_pixel )
         {
            case 1:
            {
               // gray_layout

               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit black and white interleaved image
                     typedef bit_aligned_image1_type<1, gray_layout_t>::type image_t;

                     io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                , "Views are incompatible. You might want to use read_and_convert_image()" );

                     _read_bit_aligned_view( v, boost::is_same<image_t::view_t, View >() );

                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer grayscale interleaved image
                           typedef gray8_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale interleaved image
                           typedef gray8s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit grayscale interleaved image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer grayscale interleaved image
                           typedef gray16_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer grayscale interleaved image
                           typedef gray16s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit grayscale interleaved image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer grayscale interleaved image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer grayscale interleaved image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit grayscale interleaved image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int grayscale interleaved image
                           typedef gray32_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int grayscale interleaved image
                           typedef gray32s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point grayscale interleaved image
                           typedef gray32f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit grayscale interleaved image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point grayscale interleaved image
                           typedef pixel<double, gray_layout_t>    gray64f_pixel_t;
                           typedef image< gray64f_pixel_t, false > gray64f_image_t;
                           typedef gray64f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit grayscale interleaved image
                  }
               }

               break; // samples per pixel == 1
            }

            case 3:
            {
               // rgb_layout

               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit grayscale interleaved image
                     // @todo
                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer rgb interleaved image
                           typedef rgb8_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer rgb interleaved image
                           typedef rgb8s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit rgb interleaved image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer rgb interleaved image
                           typedef rgb16_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgb interleaved image
                           typedef rgb16s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit rgb interleaved image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer rgb interleaved image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer rgb interleaved image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit rgb interleaved image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int rgb interleaved image
                           typedef rgb32_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb interleaved image
                           typedef rgb32s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb interleaved image
                           typedef rgb32f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit rgb interleaved image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point rgb interleaved image
                           typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
                           typedef image< rgb64f_pixel_t, false > rgb64f_image_t;
                           typedef rgb64f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit rgb interleaved image
                  }
               }

               break; // samples per pixel == 3
            }

            case 4:
            {
               // rgba_layout
               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit rgba interleaved image
                     // @todo
                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer grayscale interleaved image
                           typedef rgba8_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale interleaved image
                           typedef rgba8s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit rgba interleaved image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer rgba interleaved image
                           typedef rgba16_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgba interleaved image
                           typedef rgba16s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit rgba interleaved image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer rgba interleaved image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer rgba interleaved image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit rgba interleaved image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int rgb interleaved image
                           typedef rgba32_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb interleaved image
                           typedef rgba32s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb interleaved image
                           typedef rgba32f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit rgba interleaved image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point rgb interleaved image
                           typedef pixel<double, rgb_layout_t> rgba64f_pixel_t;
                           typedef image< rgba64f_pixel_t, false > rgba64f_image_t;
                           typedef rgba64f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit rgba interleaved image
                  }
               }

               break; // samples per pixel == 4
            }
         }
      }
      else if( _info._planar_configuration == PLANARCONFIG_SEPARATE )
      {
         // planar images

         switch(  _info._samples_per_pixel )
         {
            case 1:
            {
               // gray_layout

               // This branch might not much sense since there can be only one plane.

               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit grayscale image
                     // @todo
                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer grayscale image
                           typedef gray8_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale image
                           typedef gray8s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit gray image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer grayscale image
                           typedef gray16_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer grayscale image
                           typedef gray16s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit gray image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer grayscale image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer grayscale image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit gray image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int grayscale image
                           typedef gray32_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int grayscale image
                           typedef gray32s_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point grayscale image
                           typedef gray32f_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit gray image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point grayscale planar image
                           typedef pixel<double, gray_layout_t>    gray64f_pixel_t;
                           typedef image< gray64f_pixel_t, false > gray64f_planar_image_t;
                           typedef gray64f_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_interleaved_view( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit gray image
                  }
               }

               break; // samples per pixel == 1
            }

            case 3:
            {
               // rgb_layout

               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit grayscale planar image
                     // @todo
                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer rgb planar image
                           typedef rgb8_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer rgb planar image
                           typedef rgb8s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit rgb planar image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer rgb planar image
                           typedef rgb16_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgb planar image
                           typedef rgb16s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit rgb planar image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer rgb planar image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer rgb planar image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit rgb planar image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int rgb planar image
                           typedef rgb32_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb planar image
                           typedef rgb32s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb planar image
                           typedef rgb32f_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit rgb planar image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point rgb planar image
                           typedef pixel<double, rgb_layout_t> rgb64f_pixel_t;
                           typedef image< rgb64f_pixel_t, true > rgb64f_planar_image_t;
                           typedef rgb64f_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<3>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit rgb planar image
                  }
               }

               break; // samples per pixel == 3
            }

            case 4:
            {
               // rgba_layout
               switch( _info._bits_per_sample )
               {
                  case 1:
                  {
                     // 1 bit rgba planar image
                     // @todo
                     break;
                  }

                  case 8:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 8 bit unsigned integer grayscale planar image
                           typedef rgba8_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale planar image
                           typedef rgba8s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 8 bit rgba planar image
                  }

                  case 16:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 16 bit unsigned integer rgba planar image
                           typedef rgba16_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgba planar image
                           typedef rgba16s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 16 bit rgba planar image
                  }

                  case 24:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 24 bit unsigned integer rgba planar image
                           // @todo
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 24 bit unsigned integer rgba planar image
                           // @todo
                           break;
                        }
                     }

                     break; // 24 bit rgba planar image
                  }

                  case 32:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_UINT:
                        {
                           // 32 bit unsigned int rgb planar image
                           typedef rgba32_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb planar image
                           typedef rgba32s_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb planar image
                           typedef rgba32f_planar_image_t image_t;

                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 32 bit rgba planar image
                  }

                  case 64:
                  {
                     switch( _info._sample_format )
                     {
                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 64 bit floating point rgb planar image
                           typedef pixel<double, rgb_layout_t> rgba64f_pixel_t;
                           typedef image< rgba64f_pixel_t, true > rgba64f_planar_image_t;
                           typedef rgba64f_planar_image_t image_t;


                           io_error_if( views_are_compatible<View, image_t::view_t>::value != true
                                      , "Views are incompatible. You might want to use read_and_convert_image()" );

                           _read_planar_view<4>( v, is_bit_aligned<View>::type() );

                           break;
                        }
                     }

                     break; // 64 bit rgba planar image
                  }
               }

               break; // samples per pixel == 4
            }
         }
      }
      else
      {
         // planar_configuration value is incorrect.
         io_error( "Wrong planar configuration setting." );
      }
   }

   template< typename View >
   inline
   void _read_interleaved_view( const View v, boost::mpl::false_ )
   {
      tsize_t scanline_size = TIFFScanlineSize( _tiff.get() );

      std::size_t element_size = sizeof( View::value_type );

      std::size_t dd = ((std::size_t) scanline_size + element_size - 1 ) / element_size;

      std::size_t size_to_allocate = std::max( (std::size_t) _info._width
                                             , dd                          );

      std::vector< View::value_type > buffer( size_to_allocate );

      View vv = interleaved_view( _info._width
                                , 1
                                , reinterpret_cast<View::x_iterator>( &buffer.front() )
                                , size_to_allocate * element_size                         );

      for( uint32 row = 0; row < _info._height; ++row )
      {
         _read_scaline( buffer
                      , row
                      , 0
                      , _tiff );

         // copy into view
         copy_pixels( vv
                    , subimage_view( v
                                   , point_t( 0
                                            , row )
                                   , point_t( _info._width
                                            , 1              )));
      }
   }

   template< typename View >
   inline
   void _read_interleaved_view( const View v, boost::mpl::true_ )
   {
   }

   template< int Number_Of_Samples
           , typename View
           >
   inline
   void _read_planar_view( const View v, boost::mpl::false_ )
   {
      typedef nth_channel_view_type<View>::type plane_t;

      tsize_t scanline_size = TIFFScanlineSize( _tiff.get() );
      std::vector< plane_t::value_type > buffer( scanline_size );

      for( tsample_t sample = 0
         ; sample < Number_Of_Samples
         ; ++sample                    )
      {
         for( uint32 row = 0; row < _info._height; ++row )
         {
            _read_scaline( buffer
                         , row
                         , sample
                         , _tiff );

            // copy into view
            std::copy( buffer.begin()
                     , buffer.begin() + _info._width
                     , nth_channel_view( v, sample ).row_begin( row ));
         }
      }
   }

   template< int Number_Of_Samples
           , typename View
           >
   inline
   void _read_planar_view( const View v, boost::mpl::true_ )
   {
   }

   template< typename View >
   inline
   void _read_bit_aligned_view( const View& v, boost::mpl::true_ )
   {
      tsize_t scanline_size_in_bytes = TIFFScanlineSize( _tiff.get() );
      std::vector<unsigned char> buffer( scanline_size_in_bytes );

      unsigned char* first_byte = &buffer.front();
      unsigned char* last_byte  = &buffer.front() + ( scanline_size_in_bytes - 1 );
   
      View::x_iterator begin( first_byte, 0 );
      View::x_iterator end  ( last_byte , 7 );

      for( uint32 row = 0; row < _info._height; ++row )
      {
         _read_scaline( buffer
                      , row
                      , 0
                      , _tiff   );

         std::copy( begin, end, v.row_begin( row ) );
      }

   }

   template< typename View >
   inline
   void _read_bit_aligned_view( const View& v, boost::mpl::false_ )
   {
   }

   template< typename Buffer >
   inline 
   void _read_scaline( Buffer&     buffer
                     , uint32      row
                     , tsample_t   plane
                     , tiff_file_t file             )
   {
      int size = TIFFReadScanline( file.get()
                                 , &buffer.front()
                                 , row
                                 , plane           );

      io_error_if( size == -1, "Read error." );
   }


   template< typename Buffer >
   inline 
   void _read_strip( Buffer&     buffer
                   , tstrip_t    number_of_strip
                   , tsize_t     strip_size
                   , tiff_file_t file             )
   {
      int size = TIFFReadEncodedStrip( file.get()
                                     , number_of_strip
                                     , &buffer.front()
                                     , strip_size      );

      io_error_if( size == -1, "Read error." );
   }

private:

   tiff_file_t _tiff;

   basic_tiff_image_read_info _info;
};


} // detail
} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_HPP
