//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <bit>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace tts::detail
{
  inline auto as_int(float a)   noexcept  { return std::bit_cast<std::int32_t>(a); }
  inline auto as_int(double a)  noexcept  { return std::bit_cast<std::int64_t>(a); }

  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto ia = as_int(a);
    using r_t = std::remove_cvref_t<decltype(ia)>;
    constexpr auto Signmask = r_t(1) << (sizeof(r_t)*8-1);
    return std::signbit(a) ?  Signmask-ia : ia;
  }
}
