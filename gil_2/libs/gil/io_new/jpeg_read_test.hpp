#ifndef READ_TEST_HPP
#define READ_TEST_HPP

template< int Value >
struct compare
{
   template< typename Channel >
   void operator() ( const Channel& c )
   {
      assert( c == 0 );
   }
};


template< int Value >
struct eval_to
{
   template< typename Pixel >
   void operator() ( const Pixel& p )
   {
      static_for_each( p, compare<Value>() );
   }
};

void basic_jpeg_read_test();

#endif // READ_TEST_HPP
