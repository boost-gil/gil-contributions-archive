#ifndef READ_TEST_HPP
#define READ_TEST_HPP

template< int Value >
struct eval_to
{
   template< typename T >
   void operator() ( const T& v )
   {
      assert( at_c<0>( v ) == Value );
   }
};

void read_test();

#endif // READ_TEST_HPP
