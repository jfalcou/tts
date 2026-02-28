//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts::_
{
  inline auto as_int(float a)
  {
    return std::bit_cast<std::uint32_t>(a);
  }
  inline auto as_int(double a)
  {
    return std::bit_cast<std::uint64_t>(a);
  }

  template<typename T> inline auto bitinteger(T a) noexcept
  {
    auto ia             = as_int(a);
    using r_t           = decltype(ia);
    constexpr auto mask = r_t(1) << (sizeof(r_t) * 8 - 1);
    return ((ia & mask) == mask) ? mask - ia : ia;
  }
}
