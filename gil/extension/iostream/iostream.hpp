#ifndef _GIL_IOSTREAM_HPP_ 
#define _GIL_IOSTREAM_HPP_ 

#include <ostream>
#include <iomanip>


template < class PIXEL >
inline
void print( std::ostream& s, const PIXEL& pixel )
{
   for( int i = 0; i < PIXEL::num_channels; ++i )
   {
      s << pixel[i] << " ";
   }
}


#endif // _GIL_IOSTREAM_HPP_