//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#ifndef MY_REAL_HPP_INCLUDED
#define MY_REAL_HPP_INCLUDED

#include <tts/tts.hpp>

namespace n1
{
  struct my_real
  {
    float x;

    // clang-format off
    operator float() const { return x; }
    // clang-format on
  };

}

namespace tts
{
  template<> struct precision<n1::my_real>
  {
    static double ulp(n1::my_real const& a, n1::my_real const& b)
    {
      return a.x / b.x;
    }
    static double relative(n1::my_real const& a, n1::my_real const& b)
    {
      return a.x / b.x;
    }

    static double absolute(n1::my_real const& a, n1::my_real const& b)
    {
      return std::abs(a.x - b.x);
    }

    static bool ieee(n1::my_real const& a, n1::my_real const& b)
    {
      return a.x == b.x || (a.x >= 0.f && b.x >= 0.f);
    }
  };
}

#endif
