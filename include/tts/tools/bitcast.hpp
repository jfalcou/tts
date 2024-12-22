//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once
#include <string.h>
#include <cstdint>

namespace tts::_
{
  template<typename To, typename From>
  requires(sizeof(To) == sizeof(From))
  To bit_cast(const From& src)
  {
    To dst;
    memcpy(&dst, &src, sizeof(To));
    return dst;
  }

  inline auto as_int(float a)   { return bit_cast<std::uint32_t>(a); }
  inline auto as_int(double a)  { return bit_cast<std::uint64_t>(a); }

  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto  ia  = as_int(a);
    using r_t = decltype(ia);
    constexpr auto mask = r_t(1) << (sizeof(r_t)*8-1);
    return ((ia & mask) == mask) ?  mask-ia : ia;
  }
}
