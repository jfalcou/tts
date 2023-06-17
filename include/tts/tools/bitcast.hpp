//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <bit>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>

#if !defined(__cpp_lib_bit_cast)
# include <cstring>
#endif

namespace tts::detail
{
#if !defined(__cpp_lib_bit_cast)
  template <class To, class From>
  To bit_cast(const From& src) noexcept requires(sizeof(To) == sizeof(From))
  {
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
  }
#else
  using std::bit_cast;
#endif

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
