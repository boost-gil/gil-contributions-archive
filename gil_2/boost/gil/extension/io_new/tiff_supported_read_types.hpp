#ifndef TIFF_SUPPORTED_READ_TYPES_HPP
#define TIFF_SUPPORTED_READ_TYPES_HPP

/// \file
/// \brief  Defining some basic libtiff wrappers.
//
/// \author Christian Henning
///         

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
        , typename Layout
        >
struct channel_bit_sizes_factory { typedef not_allowed_t type; };

template< int BitsPerSample >
struct channel_bit_sizes_factory< BitsPerSample, gray_layout_t >
{ typedef mpl::vector1_c< unsigned, BitsPerSample > type; };

template< int BitsPerSample >
struct channel_bit_sizes_factory< BitsPerSample, rgb_layout_t >
{ typedef mpl::vector3_c< unsigned, BitsPerSample, BitsPerSample, BitsPerSample > type; };

template< int BitsPerSample >
struct channel_bit_sizes_factory< BitsPerSample, rgba_layout_t >
{ typedef mpl::vector4_c< unsigned, BitsPerSample, BitsPerSample, BitsPerSample, BitsPerSample > type; };


template< int BitsPerSample
        , int SampleFormat 
        , typename Layout  > struct pixel_type_factory { typedef not_allowed_t type; };


// gray1_pixel_t
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 1, gray_layout_t >::type
                                     , gray_layout_t, true >::value_type type;
};

// rgb1_pixel_t
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 1, rgb_layout_t >::type
                                     , rgb_layout_t, true >::value_type type;
};

// rgba1_pixel_t
template<> struct pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 1, rgba_layout_t >::type
                                     , rgba_layout_t, true >::value_type type;
};

// gray4_pixel_t
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, gray_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 4, gray_layout_t >::type
                                     , gray_layout_t, true >::value_type type;
};

// rgb4_pixel_t
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgb_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 4, rgb_layout_t >::type
                                     , rgb_layout_t, true >::value_type type;
};

// rgba4_pixel_t
template<> struct pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgba_layout_t >
{ typedef bit_aligned_pixel_reference< channel_bit_sizes_factory< 4, rgba_layout_t >::type
                                     , rgba_layout_t, true >::value_type type;
};


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
   typedef image< Pixel, IsPlanar > type; 
};

template<> struct image_type_factory< not_allowed_t, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 1, SAMPLEFORMAT_UINT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgb_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 1, SAMPLEFORMAT_UINT, rgba_layout_t >::type, true >
{ typedef not_allowed_t type; };

template<> struct image_type_factory< pixel_type_factory< 4, SAMPLEFORMAT_UINT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgb_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 4, SAMPLEFORMAT_UINT, rgba_layout_t >::type, true >
{ typedef not_allowed_t type; };

template<> struct image_type_factory< pixel_type_factory< 8, SAMPLEFORMAT_UINT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 8, SAMPLEFORMAT_INT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 16, SAMPLEFORMAT_UINT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 16, SAMPLEFORMAT_INT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 32, SAMPLEFORMAT_UINT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 32, SAMPLEFORMAT_INT, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 32, SAMPLEFORMAT_IEEEFP, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };
template<> struct image_type_factory< pixel_type_factory< 64, SAMPLEFORMAT_IEEEFP, gray_layout_t >::type, true >
{ typedef not_allowed_t type; };

} // namespace details
} // namespace gil
} // namespace boost

#endif // TIFF_SUPPORTED_READ_TYPES_HPP
