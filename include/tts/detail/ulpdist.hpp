//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_ULPDIST_HPP_INCLUDED
#define TTS_TESTS_IMPL_ULPDIST_HPP_INCLUDED

#include <cmath>
#include <type_traits>

namespace tts
{
  namespace ext
  {
    template<typename T1, typename T2 = T1, typename EnableIF = void> struct ulpdist;
  }

  template<typename T, typename U> inline double ulpdist(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      { return a == b ? 0. : 1.; }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (std::isnan(a) && std::isnan(b))) return 0.;

        if(std::isnan(a) || std::isnan(b)) return std::numeric_limits<double>::infinity();

        int e1 = 0, e2 = 0;
        T   m1, m2;
        m1 = std::frexp(a, &e1);
        m2 = std::frexp(b, &e2);

        int expo = -std::max(e1, e2);

        T e = (e1 == e2) ? std::abs(m1 - m2) : std::abs(std::ldexp(a, expo) - std::ldexp(b, expo));

        return double(e / std::numeric_limits<T>::epsilon());
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        using u_t = typename std::make_unsigned<T>::type;
        return static_cast<double>((a < b) ? u_t(b - a) : u_t(a - b));
      }
      else // External case
      {
        return ext::ulpdist<T>()(a, b);
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulpdist(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
}

#endif
