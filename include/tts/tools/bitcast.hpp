//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>

namespace tts::detail
{
  //================================================================================================
  // bit_cast till we got std::bit_cast :(
  //================================================================================================
  template<typename U, typename T> inline U bit_cast(T a) noexcept
  {
    U that;
    void const *src = reinterpret_cast<void const *>(&a);
    void *      dst = reinterpret_cast<void *>(&that);
    std::memcpy(dst, src, sizeof(a));
    return that;
  }

  inline auto as_int(float a)   noexcept  { return bit_cast<std::int32_t>(a); }
  inline auto as_int(double a)  noexcept  { return bit_cast<std::int64_t>(a); }

  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto ia = as_int(a);
    using r_t = std::remove_cvref_t<decltype(ia)>;
    constexpr auto Signmask = r_t(1) << (sizeof(r_t)*8-1);
    return std::signbit(a) ?  Signmask-ia : ia;
  }
}
