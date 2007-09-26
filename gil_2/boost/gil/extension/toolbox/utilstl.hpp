#ifndef _stl_utils_hpp_
#define _stl_utils_hpp_

// (C) Copyright Tom Brinkman 2007.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <math.h>

namespace std
{

template<typename _InIt, typename _InIt2, typename Pred> inline
_InIt find_if(_InIt _First, _InIt _Last, _InIt2 _First2, Pred pred)
{	
	for (; _First != _Last; ++_First,++_First2)
		if (pred(*_First,*_First2))
			break;
	return (_First);
}

template<typename _InIt, typename _InIt2, typename _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _InIt2 _First2, _Fn1 _Func)
{	
	for (; _First != _Last; ++_First,++_First2)
		_Func(*_First,*_First2);
	return (_Func);
}

template<typename _InIt, typename _InIt2, typename _InIt3, typename _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _InIt2 _First2, _InIt3 _First3, _Fn1 _Func)
{	
	for (; _First != _Last; ++_First,++_First2,++_First3)
		_Func(*_First, *_First2, *_First3);
	return (_Func);
}

template<typename _InIt, typename _InIt2, typename _InIt3, typename _InIt4, typename _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _InIt2 _First2, _InIt3 _First3, _InIt4 _First4, _Fn1 _Func)
{	
	for (; _First != _Last; ++_First,++_First2,++_First3,++_First4)
		_Func(*_First, *_First2, *_First3, *_First4);
	return (_Func);
}

template<typename out_t, typename _UnaryOperation> inline
void transform(std::string str, out_t out, _UnaryOperation __unary_op)
{
		std::string::iterator __first = str.begin();
		std::string::iterator __last = str.end();

      for ( ; __first != __last; ++__first, ++out)
		*out = __unary_op(*__first);
}

}

enum RoundDirection 
{
  rd_AwayZero=0,
  rd_TowardZero=1,
};

template <class A> inline
A roundTo(A val, int decimalPlaces)
{
  double mult=pow(10.0, decimalPlaces); 
  return round(val*mult)/mult;
}

template <class A> inline
A roundTo(A val, int decimalPlaces, RoundDirection rd)
{
  assert((rd==rd_AwayZero)||(rd==rd_TowardZero));

  A ret;
  double mult=pow(10.0, decimalPlaces);
  bool less0=(val<0.0 ? true : false);  

  if (less0) {
    if (rd==rd_AwayZero) {
      rd=rd_TowardZero;
    } else if (rd==rd_TowardZero) {
      rd=rd_AwayZero;
    }
  }  
  
  switch (rd) {
    case rd_AwayZero:
      ret=ceil(val*mult)/mult;
      break;
    case rd_TowardZero:
      ret=floor(val*mult)/mult;
      break;
  }
  return ret;
}

template <class A> inline
A truncTo(A val, int decimalPlaces)
{
  double mult=pow(10.0, decimalPlaces);
  return trunc(val*mult)/mult;
}

#endif
