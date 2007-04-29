#ifndef GIL_HSL_H
#define GIL_HSL_H

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {

struct hue_t {};    
struct saturation_t {};
struct lightness_t {}; 

typedef mpl::vector3< hue_t
                    , saturation_t
                    , lightness_t
                    > hsl_t;

typedef layout<hsl_t> hsl_layout_t;



GIL_DEFINE_ALL_TYPEDEFS( 32f, hsl );

template <>
struct default_color_converter_impl< rgb_t, hsl_t >
{
   template <typename P1, typename P2>
   void operator()( const P1& src, P2& dst) const
   {
   }
};

template <>
struct default_color_converter_impl<hsl_t,rgb_t>
{
   template <typename P1, typename P2>
   void operator()( const P1& src, P2& dst) const
   {
   }
};

} }  // namespace boost::gil

#endif // GIL_HSL_H
