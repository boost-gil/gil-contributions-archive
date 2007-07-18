// io_new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef boost::gil::point2<std::ptrdiff_t> point_t;

struct tiff_tag {};

// This struct has all the information that gil needs to create the appropriate image type.
// It should be a little different from file format to file format.
struct basic_tiff_image_read_info
{
   std::size_t _width;
   std::size_t _height;
   std::size_t _bits_per_pixel;
   std::size_t _bit_depth;

   // Would be nice to know the color space for fastest reading without
   // automatic conversion.
};

typedef boost::fusion::vector2< bool, unsigned long  > ulong_tag_t;
typedef boost::fusion::vector2< bool, unsigned short > ushort_tag_t;
typedef boost::fusion::vector2< bool, float > float_tag_t;

struct tiff_image_read_info
{
   ulong_tag_t  _NewSubfileType;
   ushort_tag_t _SubfileType;
   
   ushort_tag_t _BitsPerSample;
   
   ushort_tag_t _Compression;
};


using namespace std;
using namespace boost;
using namespace gil;
using namespace fusion;

int _tmain(int argc, _TCHAR* argv[])
{
   tiff_image_read_info info;

   info._NewSubfileType = ulong_tag_t( true, 1000 );

   if( at_c<0>( info._NewSubfileType ) )
   {
      cout << "write" << endl;
   }

	return 0;
}
