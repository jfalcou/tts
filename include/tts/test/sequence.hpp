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
  template<typename T> constexpr auto size(T const &c) noexcept
  {
    if constexpr(requires { c.size(); })
      return c.size();
    else if constexpr(requires { size(c); })
      return size(c);
    else
    {
      static_assert(
          requires { c.size(); } || requires { size(c); },
          "[TTS] Container type must support .size() or ADL size()");
    }
  }

  template<typename T> constexpr auto begin(T const &c) noexcept
  {
    if constexpr(requires { c.begin(); })
      return c.begin();
    else if constexpr(requires { begin(c); })
      return begin(c);
    else
    {
      static_assert(
          requires { c.begin(); } || requires { begin(c); },
          "[TTS] Container type must support .begin() or ADL begin()");
    }
  }

  template<typename T> constexpr auto end(T const &c) noexcept
  {
    if constexpr(requires { c.end(); })
      return c.end();
    else if constexpr(requires { end(c); })
      return end(c);
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
  [](auto const &local_tts_a, auto const &local_tts_b)                                             \
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
    auto ba = tts::_::begin(local_tts_a);                                                          \
    auto bb = tts::_::begin(local_tts_b);                                                          \
    auto ea = tts::_::end(local_tts_a);                                                            \
                                                                                                   \
    std::size_t i = 0;                                                                             \
    ::tts::text failures("");                                                                      \
                                                                                                   \
    while(ba != ea)                                                                                \
    {                                                                                              \
      auto local_tts_err = OP(*ba, *bb);                                                           \
      if(local_tts_err > N)                                                                        \
      {                                                                                            \
        failures += ::tts::text("      @[%ld] : %s and %s differs by %s %s.\n",                    \
                                i++,                                                               \
                                ::tts::as_text(*ba).data(),                                        \
                                ::tts::as_text(*bb).data(),                                        \
                                ::tts::as_text(local_tts_err).data(),                              \
                                UNIT);                                                             \
      }                                                                                            \
      ba++;                                                                                        \
      bb++;                                                                                        \
    }                                                                                              \
                                                                                                   \
    if(!failures.is_empty())                                                                       \
    {                                                                                              \
      FAILURE("Expected: %s == %s but found the following errors:\n%s",                            \
              TTS_STRING(SEQ1),                                                                    \
              TTS_STRING(SEQ2),                                                                    \
              failures.data());                                                                    \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
                                                                                                   \
    ::tts::global_runtime.pass();                                                                  \
    return ::tts::_::logger {false};                                                               \
  }(SEQ1, SEQ2) /**/

#define TTS_ALL(L, R, F, N, U, ...) TTS_ALL_##__VA_ARGS__(L, R, F, N, U)
#define TTS_ALL_(L, R, F, N, U) TTS_ALL_IMPL(L, R, F, N, U, TTS_FAIL)
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
  @brief Checks if all elements of two sequences are within a given absolute distance  and that
their sizes are equal.

  This comparison is performed by using the proper tts::absolute_check overload.

  @param L, R Sequences to compare.
  @param N    Maximum absolute distance accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/all_absolute.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)
#else
#  define TTS_ALL_ABSOLUTE_EQUAL(L, R, N, ...)                                                     \
    TTS_ALL(L, R, ::tts::absolute_check, N, "unit", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_RELATIVE_EQUAL
  @brief Checks if all elements of two sequences are within a given relative percentage and that
their sizes are equal.

  This comparison is performed by using the proper tts::relative_check overload.

  @param L, R Sequences to compare.
  @param N    Maximum relative percentage accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/all_relative.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)
#else
#  define TTS_ALL_RELATIVE_EQUAL(L, R, N, ...)                                                     \
    TTS_ALL(L, R, ::tts::relative_check, N, "%", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_ULP_EQUAL
  @brief Checks if all elements of two sequences are within a given ULP distance  and that their
sizes are equal.

  This comparison is performed by using the proper tts::ulp_check overload.

  @param L, R Sequences to compare.
  @param N    Maximum ULPs accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/all_ulp.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ALL_ULP_EQUAL(L, R, N, ...)
#else
#  define TTS_ALL_ULP_EQUAL(L, R, N, ...) TTS_ALL(L, R, ::tts::ulp_check, N, "ULP", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_IEEE_EQUAL
  @brief Checks if all elements of two sequences are within 0 ULP and that their sizes are equal.

  This also allow for infinites and NaNs to be compared equal if both values are the
  same infinites or are both $NaN$. This comparison is performed by using the proper tts::ulp_check
overload.

  @param L, R Sequences to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/all_ieee.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ALL_IEEE_EQUAL(L, R, ...)
#else
#  define TTS_ALL_IEEE_EQUAL(L, R, ...) TTS_ALL_ULP_EQUAL(L, R, 0, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ALL_EQUAL
  @brief Checks if all elements of two sequences are all equal and that their sizes are equal.

  This comparison is performed by using the proper tts::absolute_check overload.

  @param L, R Sequences to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/all_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ALL_EQUAL(L, R, ...)
#else
#  define TTS_ALL_EQUAL(L, R, ...) TTS_ALL_ABSOLUTE_EQUAL(L, R, 0 __VA_ARGS__)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================
