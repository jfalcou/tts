//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef MY_REAL_HPP_INCLUDED
#define MY_REAL_HPP_INCLUDED

#include <tts/tests/precision.hpp>

namespace n1
{
  struct my_real { float x; operator float() const { return x; } };
}


namespace tts { namespace ext
{
  template<typename EnableIf>
  struct ulpdist<n1::my_real,n1::my_real,EnableIf>
  {
    inline double operator()(n1::my_real const& a, n1::my_real const& b) const
    {
      return a.x/b.x;
    }
  };

  template<typename EnableIf>
  struct reldist<n1::my_real,n1::my_real,EnableIf>
  {
    inline double operator()(n1::my_real const& a, n1::my_real const& b) const
    {
      return a.x/b.x;
    }
  };

  template<typename EnableIf>
  struct absdist<n1::my_real,n1::my_real,EnableIf>
  {
    inline double operator()(n1::my_real const& a, n1::my_real const& b) const
    {
      return std::abs(a.x - b.x);
    }
  };
} }

#endif
