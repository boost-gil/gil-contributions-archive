/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_READ_TYPES_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_READ_TYPES_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief 
/// \author Christian Henning and Andreas Pokorny \n
///         
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil { namespace detail {

struct not_allowed_t {};

template< int BitsPerSample, int SampleFormat > struct channel_type_factory { typedef not_allowed_t type; };

template<> struct channel_type_factory< 8, SAMPLEFORMAT_UINT >  { typedef bits8 type; };
template<> struct channel_type_factory< 8, SAMPLEFORMAT_INT >  { typedef bits8s type; };
template<> struct channel_type_factory< 16, SAMPLEFORMAT_UINT > { typedef bits16  type; };
template<> struct channel_type_factory< 16, SAMPLEFORMAT_INT >  { typedef bits16s type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_UINT   > { typedef bits32  type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_INT    >  { typedef bits32s type; };
template<> struct channel_type_factory< 32, SAMPLEFORMAT_IEEEFP >  { typedef bits32f type; };
template<> struct channel_type_factory< 64, SAMPLEFORMAT_IEEEFP >  { typedef double type; };


template< int BitsPerSample
        , int SampleFormat 
        , typename Layout  > struct pixel_type_factory
{ typedef not_allowed_t type; };

namespace bit_aligned_pixel_types
{
   struct gray1_pixel_tag {};
   struct rgb1_pixel_tag {};
   struct rgba1_pixel_tag {};

   struct gray2_pixel_tag {};
   struct rgb2_pixel_tag {};
   struct rgba2_pixel_tag {};

   struct gray4_pixel_tag {};
   struct rgb4_pixel_tag {};
   struct rgba4_pixel_tag {};
}

// gray1_pixel_tag
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef bit_aligned_pixel_types::gray1_pixel_tag type; };

// rgb1_pixel_tag
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef bit_aligned_pixel_types::rgb1_pixel_tag type; };

// rgba1_pixel_tag
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef bit_aligned_pixel_types::rgba1_pixel_tag type; };

// gray2_pixel_tag
template<> struct pixel_type_factory< 2, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef bit_aligned_pixel_types::gray2_pixel_tag type; };

// rgb2_pixel_tag
template<> struct pixel_type_factory< 2, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef bit_aligned_pixel_types::rgb2_pixel_tag type; };

// rgba2_pixel_tag
template<> struct pixel_type_factory< 2, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef bit_aligned_pixel_types::rgba2_pixel_tag type; };

// gray4_pixel_tag
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef bit_aligned_pixel_types::gray4_pixel_tag type; };

// rgb4_pixel_tag
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef bit_aligned_pixel_types::rgb4_pixel_tag type; };

// rgba4_pixel_tag
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef bit_aligned_pixel_types::rgba4_pixel_tag type; };


// Define supported homogeneous pixel types.


// bits per sample == 8
// gray8_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_UINT >::type , gray_layout_t > type; };
// gray8s_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_INT, gray_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_INT >::type , gray_layout_t > type; };

// rgb8_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_UINT >::type, rgb_layout_t > type; };
// rgb8s_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_INT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_INT >::type, rgb_layout_t > type; };

// rgba8_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_UINT >::type, rgba_layout_t > type; };
// rgba8s_pixel_t
template<> struct pixel_type_factory< 8, SAMPLEFORMAT_INT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 8, SAMPLEFORMAT_INT >::type, rgba_layout_t > type; };

// bits per sample == 16
// gray16_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_UINT >::type , gray_layout_t > type; };
// gray16s_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_INT, gray_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_INT >::type , gray_layout_t > type; };

// rgb16_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_UINT >::type, rgb_layout_t > type; };
// rgb16s_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_INT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_INT >::type, rgb_layout_t > type; };

// rgba16_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_UINT >::type, rgba_layout_t > type; };
// rgba16s_pixel_t
template<> struct pixel_type_factory< 16, SAMPLEFORMAT_INT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 16, SAMPLEFORMAT_INT >::type, rgba_layout_t > type; };

// bits per sample == 32
// gray32_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_UINT >::type , gray_layout_t > type; };
// gray8s_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_INT, gray_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_INT >::type , gray_layout_t > type; };
// gray32f_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_IEEEFP, gray_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_IEEEFP >::type , gray_layout_t > type; };

// rgb32_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_UINT >::type, rgb_layout_t > type; };
// rgb32s_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_INT, rgb_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_INT >::type, rgb_layout_t > type; };
// rgb32f_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_IEEEFP, rgb_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_IEEEFP >::type , rgb_layout_t > type; };

// rgba32_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_UINT >::type, rgba_layout_t > type; };
// rgba32s_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_INT, rgba_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_INT >::type, rgba_layout_t > type; };
// rgba32f_pixel_t
template<> struct pixel_type_factory< 32, SAMPLEFORMAT_IEEEFP, rgba_layout_t >
{ typedef pixel< channel_type_factory< 32, SAMPLEFORMAT_IEEEFP >::type , rgba_layout_t > type; };

// bits per sample == 64
// gray64f_pixel_t
template<> struct pixel_type_factory< 64, SAMPLEFORMAT_IEEEFP, gray_layout_t >
{ typedef pixel< channel_type_factory< 64, SAMPLEFORMAT_IEEEFP >::type , gray_layout_t > type; };
// rgb64f_pixel_t
template<> struct pixel_type_factory< 64, SAMPLEFORMAT_IEEEFP, rgb_layout_t >
{ typedef pixel< channel_type_factory< 64, SAMPLEFORMAT_IEEEFP >::type , rgb_layout_t > type; };
// rgba64f_pixel_t
template<> struct pixel_type_factory< 64, SAMPLEFORMAT_IEEEFP, rgba_layout_t >
{ typedef pixel< channel_type_factory< 64, SAMPLEFORMAT_IEEEFP >::type , rgba_layout_t > type; };


template< typename Pixel, bool IsPlanar >
struct image_type_factory
{
   typedef not_allowed_t type;
};

//gray1_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::gray1_pixel_tag, false >
{ typedef bit_aligned_image1_type< 1, gray_layout_t >::type type; };
//rgb1_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgb1_pixel_tag, false >
{ typedef bit_aligned_image3_type< 1, 1, 1, rgb_layout_t >::type type; };
//rgba1_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgba1_pixel_tag, false >
{ typedef bit_aligned_image4_type< 1, 1, 1, 1, rgba_layout_t >::type type; };

//gray2_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::gray2_pixel_tag, false >
{ typedef bit_aligned_image1_type< 2, gray_layout_t >::type type; };
//rgb2_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgb2_pixel_tag, false >
{ typedef bit_aligned_image3_type< 2, 2, 2, rgb_layout_t >::type type; };
//rgba2_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgba2_pixel_tag, false >
{ typedef bit_aligned_image4_type< 2, 2, 2, 2, rgba_layout_t >::type type; };

//gray4_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::gray4_pixel_tag, false >
{ typedef bit_aligned_image1_type< 4, gray_layout_t >::type type; };
//rgb4_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgb4_pixel_tag, false >
{ typedef bit_aligned_image3_type< 4, 4, 4, rgb_layout_t >::type type; };
//rgba4_image_t
template<> struct image_type_factory< bit_aligned_pixel_types::rgba4_pixel_tag, false >
{ typedef bit_aligned_image4_type< 4, 4, 4, 4, rgba_layout_t >::type type; };

// 8-bit homogeneous image types
template<> struct image_type_factory< gray8_pixel_t, false >
{ typedef gray8_image_t type; };
template<> struct image_type_factory< rgb8_pixel_t, false >
{ typedef rgb8_image_t type; };
template<> struct image_type_factory< rgb8_pixel_t, true >
{ typedef rgb8_planar_image_t type; };
template<> struct image_type_factory< rgba8_pixel_t, false >
{ typedef rgba8_image_t type; };
template<> struct image_type_factory< rgba8_pixel_t, true >
{ typedef rgba8_planar_image_t type; };

// 16-bit homogeneous image types
template<> struct image_type_factory< gray16_pixel_t, false >
{ typedef gray16_image_t type; };
template<> struct image_type_factory< rgb16_pixel_t, false >
{ typedef rgb16_image_t type; };
template<> struct image_type_factory< rgb16_pixel_t, true >
{ typedef rgb16_planar_image_t type; };
template<> struct image_type_factory< rgba16_pixel_t, false >
{ typedef rgba16_image_t type; };
template<> struct image_type_factory< rgba16_pixel_t, true >
{ typedef rgba16_planar_image_t type; };

// 32-bit homogeneous image types
template<> struct image_type_factory< gray32_pixel_t, false >
{ typedef gray32_image_t type; };
template<> struct image_type_factory< rgb32_pixel_t, false >
{ typedef rgb32_image_t type; };
template<> struct image_type_factory< rgb32_pixel_t, true >
{ typedef rgb32_planar_image_t type; };
template<> struct image_type_factory< rgba32_pixel_t, false >
{ typedef rgba32_image_t type; };
template<> struct image_type_factory< rgba32_pixel_t, true >
{ typedef rgba32_planar_image_t type; };

// 64-bit homogeneous image types
template<> struct image_type_factory< pixel< double, gray_layout_t >, false >
{ typedef image< pixel< double, gray_layout_t >, false > type; };
template<> struct image_type_factory< pixel< double, rgb_layout_t >, false >
{ typedef image< pixel< double, rgb_layout_t >, false > type; };
template<> struct image_type_factory< pixel< double, rgb_layout_t >, true >
{ typedef image< pixel< double, rgb_layout_t >, true > type; };
template<> struct image_type_factory< pixel< double, rgba_layout_t >, false >
{ typedef image< pixel< double, rgba_layout_t >, false > type; };
template<> struct image_type_factory< pixel< double, rgba_layout_t >, true >
{ typedef image< pixel< double, rgba_layout_t >, true > type; };

} // namespace details
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_SUPPORTED_READ_TYPES_HPP_INCLUDED
