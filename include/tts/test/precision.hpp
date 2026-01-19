//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/precision.hpp>
#include <tts/engine/logger.hpp>

#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, PREC, FAILURE)                                 \
  [ & ](auto local_tts_a, auto local_tts_b)                                                        \
  {                                                                                                \
    auto r = FUNC(local_tts_a, local_tts_b);                                                       \
                                                                                                   \
    if(r <= N)                                                                                     \
    {                                                                                              \
      TTS_PASS("Expression: %s == %s within %.*g %s (over %.*g %s).",                              \
               TTS_STRING(LHS),                                                                    \
               TTS_STRING(RHS),                                                                    \
               PREC,                                                                               \
               r,                                                                                  \
               UNIT,                                                                               \
               PREC,                                                                               \
               static_cast<double>(N),                                                             \
               UNIT);                                                                              \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s == %s but %s == %s within %.*g %s when  %.*g %s were expected.",       \
              TTS_STRING(LHS),                                                                     \
              TTS_STRING(RHS),                                                                     \
              ::tts::as_text(local_tts_a).data(),                                                  \
              ::tts::as_text(local_tts_b).data(),                                                  \
              PREC,                                                                                \
              r,                                                                                   \
              UNIT,                                                                                \
              PREC,                                                                                \
              static_cast<double>(N),                                                              \
              UNIT);                                                                               \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(LHS, RHS) /**/

#define TTS_PRECISION(L, R, N, U, F, P, ...)     TTS_PRECISION_##__VA_ARGS__(L, R, N, U, F, P)
#define TTS_PRECISION_(L, R, N, U, F, P)         TTS_PRECISION_IMPL(L, R, N, U, F, P, TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L, R, N, U, F, P) TTS_PRECISION_IMPL(L, R, N, U, F, P, TTS_FATAL)

//======================================================================================================================
/**
  @defgroup test-precision Precision Tests Macros
  @brief Macros for performing tests taking floating-point precision into account.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_ABSOLUTE_EQUAL
  @brief Checks if the absolute distance between values is less or equal to a threshold

  This comparison is performed by using the proper tts::absolute_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum absolute distance accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/absolute.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ABSOLUTE_EQUAL(L, R, N, ...)
#else
#define TTS_ABSOLUTE_EQUAL(L, R, N, ...)                                                           \
  TTS_PRECISION(L, R, N, "unit", ::tts::absolute_check, 8, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_RELATIVE_EQUAL
  @brief Checks if values are within a given relative distance expressed as a percentage.

  This comparison is performed by using the proper tts::relative_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum relative percentage accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/relative.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_RELATIVE_EQUAL(L, R, N, ...)
#else
#define TTS_RELATIVE_EQUAL(L, R, N, ...)                                                           \
  TTS_PRECISION(L, R, N, "%", ::tts::relative_check, 8, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_ULP_EQUAL
  @brief Checks if two values are within a given ULP distance

  This comparison is performed by using the proper tts::ulp_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum ULPs accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/ulp.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ULP_EQUAL(L, R, N, ...)
#else
#define TTS_ULP_EQUAL(L, R, N, ...) TTS_PRECISION(L, R, N, "ULP", ::tts::ulp_check, 2, __VA_ARGS__)
#endif

#define TTS_DO_IEEE_EQUAL_IMPL(LHS, RHS, FAILURE)                                                  \
  [ & ](auto local_tts_a, auto local_tts_b)                                                        \
  {                                                                                                \
    if(::tts::ieee_check(local_tts_a, local_tts_b))                                                \
    {                                                                                              \
      TTS_PASS("Expression: %s == %s.", TTS_STRING(LHS), TTS_STRING(RHS));                         \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s == %s is false because %s != %s.",                                   \
              TTS_STRING(LHS),                                                                     \
              TTS_STRING(RHS),                                                                     \
              ::tts::as_text(local_tts_a).data(),                                                  \
              ::tts::as_text(local_tts_b).data());                                                 \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(LHS, RHS) /**/

#define TTS_DO_IEEE_EQUAL(L, R, ...)     TTS_DO_IEEE_EQUAL_##__VA_ARGS__(L, R)
#define TTS_DO_IEEE_EQUAL_(L, R)         TTS_DO_IEEE_EQUAL_IMPL(L, R, TTS_FAIL)
#define TTS_DO_IEEE_EQUAL_REQUIRED(L, R) TTS_DO_IEEE_EQUAL_IMPL(L, R, TTS_FATAL)

//======================================================================================================================
/**
  @def TTS_IEEE_EQUAL
  @brief Checks if two values are exactly within 0 ULP.

  This also allow for infinites and NaNs to be compared equal if both values are the
  same infinites or are both $NaN$. This comparison is performed by using the proper
tts::ulp_distance overload.

  @param L, R Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/ieee.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_IEEE_EQUAL(L, R, ...)
#else
#define TTS_IEEE_EQUAL(L, R, ...) TTS_DO_IEEE_EQUAL(L, R, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
