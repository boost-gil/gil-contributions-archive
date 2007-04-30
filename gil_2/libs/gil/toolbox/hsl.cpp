// hsl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/gil/extension/io/bmp_io.hpp>
#include <boost/gil/extension/toolbox/rgb_algorithms.hpp>
#include <boost/gil/extension/toolbox/hsl.hpp>

using namespace std;
using namespace boost;
using namespace gil;
using namespace toolbox;


void rgb2hsl( const rgb32f_pixel_t& p32
            , hsl32f_pixel_t&       h32 )
{
   float min_color = min( at_c<0>( p32 ), min( at_c<1>( p32 ), at_c<2>( p32 )));
   float max_color = max( at_c<0>( p32 ), max( at_c<1>( p32 ), at_c<2>( p32 )));

   if( abs( min_color - max_color ) < 0.001 )
   {
      // rgb color is gray

      get_color( h32, hue_t() ) = 0.f;
      get_color( h32, saturation_t() ) = 0.f;

      // doesn't matter which rgb channel we use.
      get_color( h32, lightness_t() ) = at_c<0>( p32 );
   }
   else
   {
      // lightness calculation

      get_color( h32, lightness_t() ) = ( min_color + max_color ) / 2.f;

      // saturation calculation

      if( get_color( h32, lightness_t() ) < 0.5f )
      {
         get_color( h32, saturation_t() ) = ( max_color - min_color ) 
                                          / ( min_color + max_color );
      }
      else
      {
         get_color( h32, saturation_t() ) = ( max_color - min_color ) 
                                          / ( 2.f - max_color - min_color );

      }

      float diff = max_color - min_color;

      // hue calculation
      if( abs( max_color - get_color( p32, red_t() )) < 0.0001f )
      {
         // max_color is red
         get_color( h32, hue_t() ) = (   get_color( p32, green_t() ) 
                                       - get_color( p32, blue_t()  )) 
                                       / diff;

      }
      else if( abs( max_color - get_color( p32, green_t() )) < 0.0001f )
      {
         // max_color is green
         // 2.0 + (b - r) / (maxColor - minColor);
         get_color( h32, hue_t() ) = 2.f +
                                    (    get_color( p32, blue_t() ) 
                                       - get_color( p32, red_t()  )) 
                                       / diff;

      }
      else if( abs( max_color - get_color( p32, blue_t() )) < 0.0001f )
      {
         // max_color is blue
         get_color( h32, hue_t() ) = 4.f +
                                    (    get_color( p32, red_t() ) 
                                       - get_color( p32, blue_t()  )) 
                                       / diff;
      }
      else
      {
         assert( false );
      }         

      get_color( h32, hue_t() ) /= 6.f;
      
      if( get_color( h32, hue_t() ) < 0.f )
      {
         get_color( h32, hue_t() ) += 1.f;
      }
   }
}


void hsl2rgb( const hsl32f_pixel_t& h32
            , rgb32f_pixel_t&       p32 )
{
   if( abs( get_color( h32, saturation_t() )) < 0.0001  )
   {
      // If saturation is 0, the color is a shade of gray
      get_color( p32, red_t()   ) = get_color( h32, lightness_t() );
      get_color( p32, green_t() ) = get_color( h32, lightness_t() );
      get_color( p32, blue_t()  ) = get_color( h32, lightness_t() );
   }
   else
   {
      float temp1, temp2;
      float tempr, tempg, tempb;

      //Set the temporary values
      if( get_color( h32, lightness_t() ) < 0.5 ) 
      {
         temp2 = get_color( h32, lightness_t() )
               * ( 1.f + get_color( h32, saturation_t() ) );
      }
      else
      {
         temp2 = ( get_color( h32, lightness_t() ) + get_color( h32, saturation_t() )) 
               - ( get_color( h32, lightness_t() ) * get_color( h32, saturation_t() ));
      }

      temp1 = 2.f
            * get_color( h32, lightness_t() ) 
            - temp2;

      tempr = get_color( h32, hue_t() ) + 1.f / 3.f;    

      if( tempr > 1.f )
      {
         tempr--;
      }

      tempg = get_color( h32, hue_t() );     
      tempb = get_color( h32, hue_t() ) - 1.f / 3.f;

      if( tempb < 0.f )
      {
         tempb++;
      }

      //Red     
      if( tempr < 1.f / 6.f )
      {
         get_color( p32, red_t() ) = temp1 + ( temp2 - temp1 ) * 6.f * tempr;
      }
      else if( tempr < 0.5f )
      {
         get_color( p32, red_t() ) = temp2;
      }
      else if( tempr < 2.f / 3.f )
      {
         get_color( p32, red_t() ) = temp1 + (temp2 - temp1) 
                                   * (( 2.f / 3.f ) - tempr) * 6.f;
      }
      else
      {
         get_color( p32, red_t() ) = temp1;
      }

      //Green       
      if( tempg < 1.f / 6.f )
      {
         get_color( p32, green_t() ) = temp1 + ( temp2 - temp1 ) * 6.f * tempg;
      }
      else if( tempg < 0.5f )
      {
         get_color( p32, green_t() ) = temp2;
      }
      else if( tempg < 2.f / 3.f )
      {
         get_color( p32, green_t() ) = temp1 + ( temp2 - temp1 )
                                     * (( 2.f / 3.f ) - tempg) * 6.f;
      }
      else
      {
         get_color( p32, green_t() ) = temp1;
      }

      //Blue    
      if( tempb < 1.f / 6.f )
      {
         get_color( p32, blue_t() ) = temp1 + (temp2 - temp1) * 6.f * tempb;
      }
      else if( tempb < 0.5f )
      {
         get_color( p32, blue_t() ) = temp2;
      }
      else if( tempb < 2.f / 3.f )
      {
         get_color( p32, blue_t() ) = temp1 + (temp2 - temp1) 
                                    * (( 2.f / 3.f ) - tempb) * 6.f;
      }
      else
      {
         get_color( p32, blue_t() ) = temp1;
      }
   }
}


int _tmain(int argc, _TCHAR* argv[])
{
   {
      gray32f_pixel_t g( 1.f );

      float red_1 = channel_convert< bits32f >( 1.f );
      float red_2 = channel_convert< bits32f >( (bits32f) 1.f );

      int i = 9;
   }


   {
      rgb8_pixel_t p( 128, 0, 128 );

      hsl32f_pixel_t h;

      color_convert( p, h );
      color_convert( h, p );

      int i = 9;
   }

   {
      size_t width  = 640;
      size_t height = 480;

      hsl32f_image_t img( width, height );
      rgb32f_image_t rgb_img( width, height );

      for( size_t y = 0; y < height; y++ )
      {
         hsl32f_view_t::x_iterator x_it = view( img ).row_begin( y );
         rgb32f_view_t::x_iterator x_rgb_it = view( rgb_img ).row_begin( y );

         float lightness = static_cast<float>( height -  y ) 
                         / height;

         for( size_t x = 0; x < width; x++ )
         {
            float hue = ( x + 1.f ) / width;

            x_it[x] = hsl32f_pixel_t( hue, 1.0, lightness );
         }
      }

      bmp_write_view( ".\\hsl_test.bmp"
                    , color_converted_view<rgb8_pixel_t>( color_converted_view<rgb32f_pixel_t>( view( img ))));
   }


   {
      rgb8_image_t rgb_img;
      bmp_read_image( ".\\flower.bmp", rgb_img );

      hsl32f_image_t hsl_img( view( rgb_img ).dimensions() );

      copy_pixels( color_converted_view<hsl32f_pixel_t>( view( rgb_img ))
                 , view( hsl_img ));

      bmp_write_view( ".\\flower_hsl.bmp"
                    , color_converted_view<rgb8_pixel_t>(view( rgb_img )));
      
   }

	return 0;
}

