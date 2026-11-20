//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/logger.hpp>
#include <tts/engine/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/precision.hpp>
#include <tts/tools/text.hpp>

namespace tts::_
{
  template<typename T> constexpr auto size(T const& c) noexcept
  {
    if constexpr(requires { c.size(); }) return c.size();
    else if constexpr(requires { size(c); }) return size(c);
    else
    {
      static_assert(
      requires { c.size(); } || requires { size(c); },
      "[TTS] Container type must support .size() or ADL size()");
    }
  }

  template<typename T> constexpr auto begin(T const& c) noexcept
  {
    if constexpr(requires { c.begin(); }) return c.begin();
    else if constexpr(requires { begin(c); }) return begin(c);
    else
    {
      static_assert(
      requires { c.begin(); } || requires { begin(c); },
      "[TTS] Container type must support .begin() or ADL begin()");
    }
  }

  template<typename T> constexpr auto end(T const& c) noexcept
  {
    if constexpr(requires { c.end(); }) return c.end();
    else if constexpr(requires { end(c); }) return end(c);
    else
    {
      static_assert(
      requires { c.end(); } || requires { end(c); },
      "[TTS] Container type must support .end() or ADL end()");
    }
  }

  template<typename T, typename U> struct failure
  {
    std::size_t index;
    T           original;
    U           other;
  };
}

#define TTS_ALL_IMPL(SEQ1, SEQ2, OP, N, UNIT, FAILURE)                                             \
  [](auto const& local_tts_a, auto const& local_tts_b)                                             \
  {                                                                                                \
    if(tts::_::size(local_tts_b) != tts::_::size(local_tts_a))                                     \
    {                                                                                              \
      FAILURE("Expected: %s == %s but found %s elements instead of %s.",                           \
              TTS_STRING(SEQ1),                                                                    \
              TTS_STRING(SEQ2),                                                                    \
              ::tts::as_text(::tts::_::size(local_tts_a)).data(),                                  \
              ::tts::as_text(::tts::_::size(local_tts_b)).data());                                 \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
                                                                                                   \
    auto        local_tts_ba = tts::_::begin(local_tts_a);                                         \
    auto        local_tts_bb = tts::_::begin(local_tts_b);                                         \
    auto        local_tts_ea = tts::_::end(local_tts_a);                                           \
                                                                                                   \
    std::size_t local_tts_i  = 0;                                                                  \
    ::tts::text local_tts_failures("");                                                            \
                                                                                                   \
    while(local_tts_ba != local_tts_ea)                                                            \
    {                                                                                              \
      auto local_tts_err = OP(*local_tts_ba, *local_tts_bb);                                       \
      if(local_tts_err > N)                                                                        \
      {                                                                                            \
        local_tts_failures += ::tts::text("      @[%ld] : %s and %s differs by %s %s.\n",          \
                                          local_tts_i++,                                           \
                                          ::tts::as_text(*local_tts_ba).data(),                    \
                                          ::tts::as_text(*local_tts_bb).data(),                    \
                                          ::tts::as_text(local_tts_err).data(),                    \
                                          UNIT);                                                   \
      }                                                                                            \
      local_tts_ba++;                                                                              \
      local_tts_bb++;                                                                              \
    }                                                                                              \
                                                                                                   \
    if(!local_tts_failures.is_empty())                                                             \
    {                                                                                              \
      FAILURE("Expected: %s == %s but found the following errors:\n%s",                            \
              TTS_STRING(SEQ1),                                                                    \
              TTS_STRING(SEQ2),                                                                    \
              local_tts_failures.data());                                                          \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
                                                                                                   \
    ::tts::global_runtime.pass();                                                                  \
    return ::tts::_::logger {false};                                                               \
  }(SEQ1, SEQ2) /**/

#define TTS_ALL(L, R, F, N, U, ...)     TTS_ALL_##__VA_ARGS__(L, R, F, N, U)
#define TTS_ALL_(L, R, F, N, U)         TTS_ALL_IMPL(L, R, F, N, U, TTS_FAIL)
#define TTS_ALL_REQUIRED(L, R, F, N, U) TTS_ALL_IMPL(L, R, F, N, U, TTS_FATAL)

//======================================================================================================================
/**
  @defgroup test-sequence Sequence Tests Macros
  @brief Macros for performing sequence-wide checks.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_ALL_ABSOLUTE_EQUAL
  @brief Checks that two sequences have the same size and that their elements are at most `N` apart.

  Measures each pair of elements with tts::absolute_check, as @ref TTS_ABSOLUTE_EQUAL does.

  @param L, R Sequences to compare, each with a `size()` member or a `size` found by
              argument-dependent lookup.
  @param N    Largest distance accepted.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/all_absolute.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)                                                       \
  TTS_ALL(L, R, ::tts::absolute_check, N, "unit", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_RELATIVE_EQUAL
  @brief Checks that two sequences have the same size and that their elements are within a combined
  relative and absolute tolerance `N`.

  Each pair of elements `a` and `b` passes if `abs(a - b) <= N * max(1, abs(a), abs(b))`, the
  [combined tolerance test](https://realtimecollisiondetection.net/blog/?p=89) of
  tts::relative_check. `N` acts as an absolute tolerance where both values are at most 1 in
  magnitude, and as a relative one elsewhere. Two NaNs or two equal infinities pass, and any other
  NaN or infinity fails.

  `N` is a ratio, so `0.005` accepts half a percent. On an arithmetic type, a finite `N` of 1 or
  more fails the check with a message asking for `N` divided by a hundred, since TTS 3 took a
  percentage.

  @param L, R Sequences to compare, each with a `size()` member or a `size` found by
              argument-dependent lookup.
  @param N    Tolerance, as a ratio.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/all_relative.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)                                                       \
  (                                                                                                \
  ::tts::_::reads_as_percent<decltype(*::tts::_::begin(L))>(N)                                     \
  ? TTS_PERCENT_TOLERANCE_##__VA_ARGS__(N)                                                         \
  : TTS_ALL(L, R, ::tts::relative_check, N, "rel", __VA_ARGS__))
#endif

//======================================================================================================================
/**
  @def TTS_ALL_ULP_EQUAL
  @brief Checks that two sequences have the same size and that their elements are at most `N` ULP
  apart.

  Measures each pair of elements with tts::ulp_check, as @ref TTS_ULP_EQUAL does.

  @param L, R Sequences to compare, each with a `size()` member or a `size` found by
              argument-dependent lookup.
  @param N    Largest distance accepted, in ULP.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/all_ulp.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_ULP_EQUAL(L, R, N, ...)
#else
#define TTS_ALL_ULP_EQUAL(L, R, N, ...) TTS_ALL(L, R, ::tts::ulp_check, N, "ULP", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_IEEE_EQUAL
  @brief Checks that two sequences have the same size and that their elements are 0 ULP apart, that
  is equal or both NaN.

  Compares the elements as @ref TTS_ALL_ULP_EQUAL does with `N` = 0.

  @param L, R Sequences to compare, each with a `size()` member or a `size` found by
              argument-dependent lookup.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/all_ieee.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_IEEE_EQUAL(L, R, ...)
#else
#define TTS_ALL_IEEE_EQUAL(L, R, ...) TTS_ALL_ULP_EQUAL(L, R, 0, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_EQUAL
  @brief Checks that two sequences have the same size and equal elements.

  Compares the elements as @ref TTS_ALL_ABSOLUTE_EQUAL does with `N` = 0.

  @param L, R Sequences to compare, each with a `size()` member or a `size` found by
              argument-dependent lookup.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/all_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ALL_EQUAL(L, R, ...)
#else
#define TTS_ALL_EQUAL(L, R, ...) TTS_ALL_ABSOLUTE_EQUAL(L, R, 0, __VA_ARGS__)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================
