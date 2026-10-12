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

    static bool bit_equal(L const& l, R const& r)
    {
      static_assert(sizeof(L) == sizeof(R), "Types must have the same size for bitwise comparison");

      return std::memcmp(&l, &r, sizeof(L)) == 0;
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
    @brief When two values are equal, when one comes before the other, and when their bits agree

    Specialize this for your own types rather than overloading the free functions: a specialization
    that does not match is a compilation error, where a misnamed overload used to be ignored in
    silence.

    The three members are `equal`, `less` and `bit_equal`. Write the ones the type needs and leave
    the rest out: their defaults are `operator==`, `operator<` and a `memcmp` over the whole
    object. That last one reads padding, so a type whose storage is wider than the value it
    carries, such as a SIMD register wider than the lanes it holds, has to write `bit_equal`.

    Both operand types are parameters, in the order they are written, so a comparison between two
    different types is specialized on the pair rather than on either side of it.

    The `compare_equal` and `compare_less` free functions this replaces are gone. An overload left
    behind is reported where it would have been used, rather than ignored in silence.

    @tparam L Type of the left-hand operand
    @tparam R Type of the right-hand operand, the same as L unless stated otherwise

    @groupheader{Example}
    @snippet doc/comparators.cpp snippet1

    A specialization that answers for `less` as well covers the ordering macros.

    @snippet doc/comparators.cpp snippet2

    A type whose storage is wider than the value it carries answers for `bit_equal`.

    @snippet doc/comparators.cpp snippet3
  **/
  //====================================================================================================================
  template<typename L, typename R = L> struct comparison
  {
  };

  //! @}
}

namespace tts::_
{
  // The fallback refuses a member that is there but does not take the operands.
  struct bit_eq_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      if constexpr(requires { comparison<L, R>::bit_equal(l, r); })
      {
        return comparison<L, R>::bit_equal(l, r);
      }
      else
      {
        static_assert(
        !requires { comparison<L, R>::bit_equal; },
        "[TTS] tts::comparison<L, R>::bit_equal does not accept the two operands.");

        return builtin_comparison<L, R>::bit_equal(l, r);
      }
    }
  };

  inline constexpr bit_eq_t bit_eq {};

  struct bit_neq_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      return !bit_eq(l, r);
    }
  };

  inline constexpr bit_neq_t bit_neq {};

  struct eq_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      if constexpr(requires { comparison<L, R>::equal(l, r); })
      {
        return comparison<L, R>::equal(l, r);
      }
      else
      {
        static_assert(
        !requires { comparison<L, R>::equal; },
        "[TTS] tts::comparison<L, R>::equal does not accept the two operands.");

        return builtin_comparison<L, R>::equal(l, r);
      }
    }
  };

  inline constexpr eq_t eq {};

  struct neq_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      return !eq(l, r);
    }
  };

  inline constexpr neq_t neq {};

  struct lt_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      if constexpr(requires { comparison<L, R>::less(l, r); })
      {
        return comparison<L, R>::less(l, r);
      }
      else
      {
        static_assert(
        !requires { comparison<L, R>::less; },
        "[TTS] tts::comparison<L, R>::less does not accept the two operands.");

        return builtin_comparison<L, R>::less(l, r);
      }
    }
  };

  inline constexpr lt_t lt {};

  struct le_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      return lt(l, r) || eq(l, r);
    }
  };

  inline constexpr le_t le {};

  struct gt_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      return !le(l, r);
    }
  };

  inline constexpr gt_t gt {};

  struct ge_t
  {
    template<typename L, typename R> constexpr bool operator()(L const& l, R const& r) const
    {
      return !lt(l, r);
    }
  };

  inline constexpr ge_t ge {};
}
