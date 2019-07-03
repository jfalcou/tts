//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//====================================================================================================
#ifndef TTS_TESTS_IMPL_ABSDIST_HPP_INCLUDED
#define TTS_TESTS_IMPL_ABSDIST_HPP_INCLUDED

#include <type_traits>
#include <cmath>

namespace tts
{
  namespace ext
  {
    template<typename T1, typename T2 = T1, typename EnableIF = void> struct absdist;
  }

  template<typename T, typename U> inline double absdist(T const& a, U const& b)
  {
    if constexpr( std::is_same_v<T,U>)
    {
      if constexpr( std::is_same_v<T,bool> )  // Boolean case
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr( std::is_floating_point_v<T> )  // IEEE cases
      {
        if((a == b) || (std::isnan(a) && std::isnan(b))) return 0.;

        if(std::isinf(a) || std::isinf(b) || std::isnan(a) || std::isnan(b))
          return std::numeric_limits<double>::infinity();

        return std::abs(a - b);
      }
      else if constexpr( std::is_integral_v<T> && !std::is_same_v<T, bool> ) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absdist(d0, d1);
      }
      else  // External case
      {
        return ext::absdist<T>()(a,b);
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absdist(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }
}

#endif
