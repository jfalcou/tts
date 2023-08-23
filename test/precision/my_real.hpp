//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#ifndef MY_REAL_HPP_INCLUDED
#define MY_REAL_HPP_INCLUDED

#include <tts/tts.hpp>

namespace n1
{
  struct my_real { float x; operator float() const { return x; } };
}

namespace tts
{
  inline double ulp_distance(n1::my_real const &a, n1::my_real const &b)
  {
    return a.x/b.x;
  }

  inline bool is_ieee_equal(n1::my_real const &a, n1::my_real const &b)
  {
    return a.x == b.x || (a.x >= 0.f && b.x >= 0.f);
  }

  inline double relative_distance(n1::my_real const &a, n1::my_real const &b)
  {
    return a.x/b.x;
  }

  inline double absolute_distance(n1::my_real const &a, n1::my_real const &b)
  {
    return std::abs(a.x - b.x);
  }
}

#endif
