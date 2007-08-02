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
                           // 8 bit unsigned integer grayscale interleaved image
                           typedef gray8_image_t   image_t;
                           typedef image_t::view_t view_t;
                           typedef num_planes< view_t, is_planar<view_t>::type >::type num_plane_t;

                           tsize_t strip_size     = TIFFStripSize     ( _tiff.get() );
                           tsize_t max_strips     = TIFFNumberOfStrips( _tiff.get() );
                           tsize_t rows_per_strip = strip_size / _info._width;

                           tsize_t element_size     = sizeof( View::value_type );
                           tsize_t size_to_allocate = strip_size / element_size;

                           std::vector< View::value_type > buffer( size_to_allocate );

                           std::size_t y = 0;
                           for( tsize_t strip_count = 0; strip_count < max_strips; ++strip_count )
                           {
                              _read_strip( buffer
                                         , strip_count
                                         , strip_size
                                         , _tiff      );

                              // copy into view
                              for( tsize_t y = 0; y < rows_per_strip; ++y )
                              {
                                 tsize_t row = ( strip_count * rows_per_strip ) + y;

                                 if( row < ( tsize_t ) _info._height )
                                 {
                                    std::copy( buffer.begin() +    ( y * _info._width )
                                             , buffer.begin() + ((( y + 1 ) * _info._width ) - 1 )
                                             , v.row_begin( row ));
                                 }
                              }
                           }

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale interleaved image
                           typedef gray8s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer grayscale interleaved image
                           typedef gray16s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int grayscale interleaved image
                           typedef gray32s_image_t image_t;

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point grayscale interleaved image
                           typedef gray32f_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer rgb interleaved image
                           typedef rgb8s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgb interleaved image
                           typedef rgb16s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb interleaved image
                           typedef rgb32s_image_t image_t;

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb interleaved image
                           typedef rgb32f_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale interleaved image
                           typedef rgba8s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgba interleaved image
                           typedef rgba16s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb interleaved image
                           typedef rgba32s_image_t image_t;

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb interleaved image
                           typedef rgba32f_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale image
                           typedef gray8s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer grayscale image
                           typedef gray16s_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int grayscale image
                           typedef gray32s_image_t image_t;

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point grayscale image
                           typedef gray32f_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer rgb planar image
                           typedef rgb8s_planar_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgb planar image
                           typedef rgb16s_planar_image_t image_t;

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

                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb planar image
                           typedef rgb32s_planar_image_t image_t;

                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb planar image
                           typedef rgb32f_planar_image_t image_t;

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
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 8 bit signed integer grayscale planar image
                           typedef rgba8s_planar_image_t image_t;
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
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 16 bit signed integer rgba planar image
                           typedef rgba16s_planar_image_t image_t;
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
                           break;
                        }

                        case SAMPLEFORMAT_INT:
                        {
                           // 32 bit signed int rgb planar image
                           typedef rgba32s_planar_image_t image_t;
                           break;
                        }

                        case SAMPLEFORMAT_IEEEFP:
                        {
                           // 32 bit floating point rgb planar image
                           typedef rgba32f_planar_image_t image_t;
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

         io_error( "Read error." );
      }
   }

   // Reads directly into the view.
   template< typename View >
   void _read_2( const View& v )
   {
      typedef num_planes< View, is_planar<View>::type >::type num_plane_t;

      if( _info._planar_configuration == PLANARCONFIG_CONTIG )
      {
         // interleave image
         _read_data( v, is_planar<View>::type(), num_plane_t() );
      }
      else if( _info._planar_configuration == PLANARCONFIG_SEPARATE )
      {
         // planar image
         _read_data( v, is_planar<View>::type(), num_plane_t() );
      }
      else
      {
         // something is wrong.
      }
   }


   template< typename View >

   void read_interleaved_data( std::vector<unsigned char>& buffer )
   {
      tsize_t strip_size = TIFFStripSize     ( _tiff.get() );
      tsize_t max_strips = TIFFNumberOfStrips( _tiff.get() );

      unsigned char* buffer = reinterpret_cast<unsigned char*>( interleaved_view_get_raw_data( v ));

      unsigned int offset = 0;
      for( tsize_t strip_count = 0; strip_count < max_strips; ++strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                        , strip_count
                                        , buffer + offset
                                        , strip_size      );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }
   }

   template< typename View >
   void read_planar_data( const View& v, boost::mpl::int_<3> )
   {
      tsize_t strip_size = TIFFStripSize     ( _tiff.get() );
      tsize_t max_strips = TIFFNumberOfStrips( _tiff.get() );

      int red_strip_count   = 0;
      int red_last_strip    = max_strips / 3;
      int green_strip_count = red_last_strip;
      int green_last_strip  = 2 * max_strips / 3;
      int blue_strip_count  = green_last_strip;
      int blue_last_strip   = max_strips;

      unsigned char* red   = reinterpret_cast<unsigned char*>( planar_view_get_raw_data( v, 0 ));
      unsigned char* green = reinterpret_cast<unsigned char*>( planar_view_get_raw_data( v, 1 ));
      unsigned char* blue  = reinterpret_cast<unsigned char*>( planar_view_get_raw_data( v, 2 ));


      unsigned int offset = 0;
      for( ; red_strip_count < red_last_strip; ++red_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , red_strip_count
                                       , red + offset
                                       , strip_size );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }

      offset = 0;
      for( ; green_strip_count < green_last_strip; ++green_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , green_strip_count
                                       , green + offset
                                       , strip_size         );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }


      offset = 0;
      for( ; blue_strip_count < blue_last_strip; ++blue_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , blue_strip_count
                                       , blue + offset
                                       , strip_size         );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }
   }

   template< typename Buffer >
   void read_planar_data( Buffer& buffer, boost::mpl::int_<4> )
   {
/*
      tsize_t strip_size = TIFFStripSize     ( _tiff.get() );
      tsize_t max_strips = TIFFNumberOfStrips( _tiff.get() );

      int red_strip_count   = 0;
      int red_last_strip    = max_strips / 4;
      int green_strip_count = red_last_strip;
      int green_last_strip  = 2 * max_strips / 4;
      int blue_strip_count  = green_last_strip;
      int blue_last_strip   = 3 * max_strips / 4;
      int alpha_strip_count  = blue_last_strip;
      int alpha_last_strip   = max_strips;


      unsigned int offset = 0;
      for( ; red_strip_count < red_last_strip; ++red_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , red_strip_count
                                       , red + offset
                                       , strip_size );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }

      offset = 0;
      for( ; green_strip_count < green_last_strip; ++green_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , green_strip_count
                                       , green + offset
                                       , strip_size         );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }


      offset = 0;
      for( ; blue_strip_count < blue_last_strip; ++blue_strip_count )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                       , blue_strip_count
                                       , blue + offset
                                       , strip_size         );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }

      offset = 0;
      for( ; alpha_strip_count < alpha_last_strip; ++alpha_strip_count )
      {
         _read_strip(  )

         offset += size;
      }
*/
   }

/*   inline
   void _read_strips( std::vector<unsigned char>& buffer
                    , std::size_t                 first_strip_number
                    , std::size_t                 last_strip_number
                    , std::size_t                 strip_size
                    , tiff_file_t                 file             )
   {
      std::size_t offset = 0;
      for( ; first_strip_number < last_strip_number; ++first_strip_number )
      {
         int size = TIFFReadEncodedStrip( _tiff.get()
                                        , first_strip_number
                                        , &buffer.front() + offset
                                        , strip_size                );

         io_error_if( size == -1, "Read error." );

         offset += size;
      }
   }
*/

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
