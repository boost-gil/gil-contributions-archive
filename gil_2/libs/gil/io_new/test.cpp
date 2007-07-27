#include "stdafx.h"

#include <boost/any.hpp>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

using namespace boost::gil;

template< typename Image >
void read_image( Image& img,  )
{
}

int main(int argc, char* argv[])
{

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 1     )
   {
      // 1 bit grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 8
     && channel_type     == SAMPLEFORMAT_UINT )
   {
      // 8 bit unsigned integer grayscale interleaved image
   }
   
   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 8
     && channel_type     == SAMPLEFORMAT_INT )
   {
      // 8 bit unsigned integer grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 16
     && channel_type     == SAMPLEFORMAT_UINT )
   {
      // 16 bit unsigned integer grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 16
     && channel_type     == SAMPLEFORMAT_INT )
   {
      // 16 bit signed integer grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 24
     && channel_type     == SAMPLEFORMAT_UINT )
   {
      // 24 bit unsigned integer grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 24
     && channel_type     == SAMPLEFORMAT_INT )
   {
      // 24 bit unsigned integer grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 32
     && channel_type     == SAMPLEFORMAT_UINT )
   {
      // 32 bit unsigned int grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 32
     && channel_type     == SAMPLEFORMAT_INT )
   {
      // 32 bit signed int grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 32
     && channel_type     == SAMPLEFORMAT_IEEEFP )
   {
      // 32 bit floating point grayscale interleaved image
   }

   if(  planar           == false
     && samples_of_pixel == 1
     && bits_per_sample  == 64
     && channel_type     == SAMPLEFORMAT_IEEEFP )
   {
      // 32 bit floating point grayscale interleaved image
   }

   return 0;
}