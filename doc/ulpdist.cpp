//==================================================================================================
/*
  Copyright 2018 Joel FALCOU
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
template<class T> double ulpdist(T a0, T a1)
{
  // Exit if a0 and a1 are equal or both NaN
  if( (a0 == a1) || ((a0!=a0) && (a1!=a1)) )
    return 0.;

  int e1,e2;
  T   m1,m2;
  m1 = std::frexp(a0, &e1);
  m2 = std::frexp(a1, &e2);

  // Compute the largest exponent between arguments
  int expo = std::max(e1, e2);

  // Properly normalize the two numbers by the same factor in a way
  // that the largest of the two numbers exponents will be brought to zero
  T n1 = std::ldexp(a0, -expo);
  T n2 = std::ldexp(a1, -expo);

  // Compute the absolute difference of the normalized numbers
  T e = (e1 == e2) ? std::abs(m1-m2) : std::abs(n1-n2);

  // Return the distance in ULP by diving this difference by the machine epsilon
  return double(e/std::numeric_limits<T>::epsilon());
}
