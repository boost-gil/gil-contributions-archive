#ifndef JPEG_SUPPORTED_READ_TYPES_HPP
#define JPEG_SUPPORTED_READ_TYPES_HPP

/// \file
/// \brief
//
/// \author Christian Henning
///         

namespace boost { namespace gil { namespace detail {

struct not_allowed_t {};

template< int NumComponents, typename Layout >
struct image_type_factory
{
   typedef not_allowed_t type;
};

template<> struct image_type_factory< 1, gray_layout_t >
{ typedef gray8_image_t type; };

template<> struct image_type_factory< 3, rgb_layout_t >
{ typedef gray8_image_t type; };

template<> struct image_type_factory< 4, cmyk_layout_t >
{ typedef gray8_image_t type; };

} // namespace details
} // namespace gil
} // namespace boost

#endif // JPEG_SUPPORTED_READ_TYPES_HPP
