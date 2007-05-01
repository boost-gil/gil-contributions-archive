#ifndef GIL_HSV_H
#define GIL_HSV_H

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {

namespace hsv_color_space
{
struct hue_t {};    
struct saturation_t{};
struct value_t {}; 
}

typedef mpl::vector3< hsv_color_space::hue_t
                    , hsv_color_space::saturation_t
                    , hsv_color_space::value_t
                    > hsv_t;

typedef layout<hsv_t> hsv_layout_t;


GIL_DEFINE_ALL_TYPEDEFS( 32f, hsv );

template <>
struct default_color_converter_impl< rgb_t, hsv_t >
{
   template <typename P1, typename P2>
   void operator()( const P1& src, P2& dst ) const
   {
      using namespace hsv_color_space;

      // only bits32f for hsv is supported
      bits32f temp_red   = channel_convert<bits32f>( get_color( src, red_t()   ));
      bits32f temp_green = channel_convert<bits32f>( get_color( src, green_t() ));
      bits32f temp_blue  = channel_convert<bits32f>( get_color( src, blue_t()  ));

      bits32f hue, saturation, value;

      bits32f min_color = min( temp_red, min( temp_green, temp_blue ));
      bits32f max_color = max( temp_red, max( temp_green, temp_blue ));

      value = max_color;

      bits32f diff = max_color - min_color;

      if( max_color < 0.0001 )
      {  
         saturation = 0.f;
      }
      else  
      {      
         saturation = diff / max_color;
      }


      if( saturation < 0.0001 )
      {
         //it doesn't matter what value it has
         hue = 0.f; 
      }   
      else
      { 
         if( abs( temp_red - max_color ) < 0.0001 )
         {
            hue = ( temp_green - temp_blue )
                / diff;
         }
         else if( temp_green == max_color )
         {
            hue = 2.f + ( temp_blue - temp_red ) 
                / diff;
         }
         else
         {
            hue = 4.f + ( temp_red - temp_green ) 
                / diff;
         }

         //to bring it to a number between 0 and 1
         hue /= 6.f; 

         if( hue < 0.f )
         {
            hue++;
         }
      }

      get_color( dst, hue_t() )        = hue;
      get_color( dst, saturation_t() ) = saturation;
      get_color( dst, value_t() )      = value;
   }
};

template <>
struct default_color_converter_impl<hsv_t,rgb_t>
{
   template <typename P1, typename P2>
   void operator()( const P1& src, P2& dst) const
   {
      using namespace hsv_color_space;

      bits32f red, green, blue;

      //If saturation is 0, the color is a shade of gray
      if( abs( get_color( src, saturation_t() )) < 0.0001  )
      {
         // If saturation is 0, the color is a shade of gray
         red   = get_color( src, value_t() );
         green = get_color( src, value_t() );
         blue  = get_color( src, value_t() );
      }
      else
      {
         bits32f frac, p, q, t, h;
         bits32 i;

         //to bring hue to a number between 0 and 6, better for the calculations
         h = get_color( src, hue_t() );
         h *= 6.f;

         i = static_cast<bits32>( floor( h ));

         frac = h - i;

         p = get_color( src, value_t() ) 
           * ( 1.f - get_color( src, saturation_t() ));

         q = get_color( src, value_t() )
           * ( 1.f - ( get_color( src, saturation_t() ) * frac ));

         t = get_color( src, value_t() ) 
           * ( 1.f - ( get_color( src, saturation_t() ) * ( 1.f - frac )));

         switch( i )
         {         
            case 0: 
            {
               red   = get_color( src, value_t() );
               green = t;
               blue  = p;

               break;
            }

            case 1: 
            {
               red   = q;
               green = get_color( src, value_t() );
               blue  = p;

               break;
            }

            case 2: 
            {
               red   = p;
               green = get_color( src, value_t() );
               blue  = t;

               break;
            }

            case 3: 
            {
               red   = p;
               green = q;
               blue  = get_color( src, value_t() );

               break;
            }

            case 4: 
            {
               red   = t;
               green = p;
               blue  = get_color( src, value_t() );

               break;
            }

            case 5: 
            {
               red   = get_color( src, value_t() );
               green = p; 
               blue  = q;

               break;
            }

         }
      }

      get_color(dst,red_t())  =
         channel_convert<typename color_element_type<red_t,  P2>::type>( red );
      get_color(dst,green_t())=
         channel_convert<typename color_element_type<green_t,P2>::type>( green );
      get_color(dst,blue_t()) =
         channel_convert<typename color_element_type<blue_t, P2>::type>( blue );
   }
};

} }  // namespace boost::gil

#endif // GIL_HSV_H