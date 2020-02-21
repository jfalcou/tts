//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_ULPDIST_HPP_INCLUDED
#define TTS_DETAIL_ULPDIST_HPP_INCLUDED

#include <tts/detail/bits.hpp>
#include <type_traits>
#include <cstring>
#include <cmath>

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
      {
        return a == b ? 0. : 0.5;
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;

        if((a == b) || (std::isnan(a) && std::isnan(b)))
        {
          return 0.;
        }
        else if (std::isunordered(a, b))
        {
          return std::numeric_limits<double>::infinity();
        }
        else
        {
          auto aa = detail::bitinteger(a);
          auto bb = detail::bitinteger(b);

          if(aa > bb) std::swap(aa, bb);

          auto z = static_cast<ui_t>(bb-aa);

          if( std::signbit(a) ^ std::signbit(b) ) ++z;
          return z/2.;
        }
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        using u_t = typename std::make_unsigned<T>::type;

        // TODO: Fix overflow in case of very huge integral value
        return ((a < b) ? u_t(b - a) : u_t(a - b))/2.;
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
