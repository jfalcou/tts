//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

//==================================================================================================
// Wrappers for comparisons with external setup
//==================================================================================================
namespace tts::detail
{
  template<typename L, typename R>
  concept comparable_equal  = requires(L l, R r) { compare_equal(l,r); };

  template<typename L, typename R>
  concept comparable_less   = requires(L l, R r) { compare_less(l,r); };

  template<typename L, typename R> inline constexpr bool eq(L const &l, R const &r)
  {
    if constexpr( comparable_equal<L,R> ) return compare_equal(l,r);
    else                                  return l == r;
  }

  template<typename L, typename R> inline constexpr bool lt(L const &l, R const &r)
  {
    if constexpr( comparable_less<L,R> )  return compare_less(l,r);
    else                                  return l < r;
  }
}
