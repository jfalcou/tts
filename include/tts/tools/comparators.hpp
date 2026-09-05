//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

//======================================================================================================================
// Wrappers for comparisons with external setup
//======================================================================================================================
namespace tts::_
{
  template<typename L, typename R> struct builtin_comparison
  {
    static constexpr bool equal(L const& l, R const& r)
    {
      static_assert(
      !requires { compare_equal(l, r); },
      "[TTS] tts::compare_equal is no longer a customization point. "
      "Specialize tts::comparison<L, R>::equal instead.");

      return l == r;
    }

    static constexpr bool less(L const& l, R const& r)
    {
      static_assert(
      !requires { compare_less(l, r); },
      "[TTS] tts::compare_less is no longer a customization point. "
      "Specialize tts::comparison<L, R>::less instead.");

      return l < r;
    }
  };
}

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-comparison Comparison Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /*!
    @ingroup tools-comparison
    @brief When two values count as equal, and when one comes before the other

    Specialize this for your own types rather than overloading the free functions: a specialization
    that does not match is a compilation error, where a misnamed overload used to be ignored in
    silence. Inherit from tts::_::builtin_comparison<L, R> to keep the member you leave alone.

    Both operand types are parameters, in the order they are written, so a comparison between two
    different types is specialized on the pair rather than on either side of it.

    The `compare_equal` and `compare_less` free functions this replaces are gone. An overload left
    behind is reported where it would have been used, rather than ignored in silence.

    @tparam L Type of the left-hand operand
    @tparam R Type of the right-hand operand, the same as L unless stated otherwise
  **/
  //====================================================================================================================
  template<typename L, typename R = L> struct comparison : _::builtin_comparison<L, R>
  {
  };

  //! @}
}

namespace tts::_
{
  template<typename L, typename R> inline constexpr bool bit_eq(L const& l, R const& r)
  {
    static_assert(sizeof(L) == sizeof(R), "Types must have the same size for bitwise comparison");

    return std::memcmp(&l, &r, sizeof(L)) == 0;
  }

  template<typename L, typename R> inline constexpr bool bit_neq(L const& l, R const& r)
  {
    static_assert(sizeof(L) == sizeof(R), "Types must have the same size for bitwise comparison");

    return std::memcmp(&l, &r, sizeof(L)) != 0;
  }

  template<typename L, typename R> inline constexpr bool eq(L const& l, R const& r)
  {
    return comparison<L, R>::equal(l, r);
  }

  template<typename L, typename R> inline constexpr bool neq(L const& l, R const& r)
  {
    return !eq(l, r);
  }

  template<typename L, typename R> inline constexpr bool lt(L const& l, R const& r)
  {
    return comparison<L, R>::less(l, r);
  }

  template<typename L, typename R> inline constexpr bool le(L const& l, R const& r)
  {
    return lt(l, r) || eq(l, r);
  }

  template<typename L, typename R> inline constexpr bool gt(L const& l, R const& r)
  {
    return !le(l, r);
  }

  template<typename L, typename R> inline constexpr bool ge(L const& l, R const& r)
  {
    return !lt(l, r);
  }
}
