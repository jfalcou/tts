//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_BITS_HPP_INCLUDED
#define TTS_DETAIL_BITS_HPP_INCLUDED

#include <type_traits>
#include <cstring>
#include <cmath>

namespace tts::detail
{
  template<typename U, typename T> inline U bit_cast(T a) noexcept
  {
    U that;

    void const *src = reinterpret_cast<void const *>(&a);
    void *      dst = reinterpret_cast<void *>(&that);

    std::memcpy(dst, src, sizeof(a));

    return that;
  }

  template<typename T> inline auto as_int(T a) noexcept
  {
    using Target = std::conditional_t<std::is_same_v<T, float>,std::int32_t, std::int64_t>;
    return bit_cast<Target>(a);
  }

  template<typename T> inline auto as_floating( T a) noexcept
  {
    using Target = std::conditional_t<std::is_same_v<T, std::int64_t>,double,float>;
    return bit_cast<Target>(a);
  }

  template<typename T> inline auto bitinteger( T a) noexcept
  {
    using r_t = std::conditional_t<std::is_same_v<T, float>, std::int32_t, std::int64_t>;
    constexpr r_t Signmask = r_t(1) << (sizeof(r_t)*8-1);

    auto ia = as_int(a);

    return std::signbit(a) ?  Signmask-ia : ia;
  }

  template<typename T> inline auto bitfloating( T a) noexcept
  {
    constexpr auto s = T(1) << (sizeof(T)*8-1);
    return as_floating( a>=0 ? a : s-a );
  }

  template<typename T> inline auto next(T a, int n=1) noexcept
  {
    if constexpr(std::is_floating_point_v<T>)
    {
      auto constexpr is_positive = [](auto x) { return !std::signbit(x); };
      auto constexpr is_negative = [](auto x) { return  std::signbit(x); };
      auto pz = bitinteger(a) + n - 1;
      auto z  = bitfloating( pz + 1 );

      return (is_negative(a) && is_positive(z)) ? (z == 0 ? T(-0.) : bitfloating(pz))
                                                : z;
    }
    else
    {
      return static_cast<T>(a+n);
    }
  }

  template<typename T> inline auto prev(T a) noexcept
  {
    if constexpr(std::is_floating_point_v<T>)
    {
      auto constexpr is_positive = [](auto x) { return !std::signbit(x); };
      auto pz = bitinteger(a) - 1;
      auto z  = bitfloating(pz - 1);

      return (a==0) && is_positive(a) ? T(-0.) : z;
    }
    else
    {
      return static_cast<T>(a-1);
    }
  }

  template<typename T> inline auto nb_values(T a, T b) noexcept
  {
    if constexpr(std::is_floating_point_v<T>)
    {
      using r_t = std::conditional_t<std::is_same_v<float,T>,std::uint32_t,std::uint64_t>;
      if( std::isnan(a) || std::isnan(b) )
      {
        return std::numeric_limits<r_t>::max();
      }
      else
      {
        auto aa = bitinteger(a);
        auto bb = bitinteger(b);
        r_t   n = nb_values(aa,bb);

        n += (std::signbit(a) != std::signbit(b)) ? 1 : 0;
        return n;
      }
    }
    else
    {
      return std::make_unsigned_t<T>(std::max(a,b) - std::min(a,b));
    }
  }
}

#endif
