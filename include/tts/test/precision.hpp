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
#include <tts/engine/check.hpp>
#include <tts/tools/as_text.hpp>

namespace tts::_::measure
{
  struct absolute
  {
    template<typename A, typename B> static double of(A const& a, B const& b)
    {
      return ::tts::absolute_check(a, b);
    }
  };

  struct relative
  {
    template<typename A, typename B> static double of(A const& a, B const& b)
    {
      return ::tts::relative_check(a, b);
    }
  };

  struct ulp
  {
    template<typename A, typename B> static double of(A const& a, B const& b)
    {
      return ::tts::ulp_check(a, b);
    }
  };
}

namespace tts::_
{
  template<typename Measure, typename A, typename B, typename N>
  logger check_precision(A const&          a,
                         B const&          b,
                         N const&          n,
                         char const*       unit,
                         int               digits,
                         check_site const& site)
  {
    double distance = Measure::of(a, b);

    if(distance <= n)
    {
      if(check_passed())
        check_report(site,
                     ::tts::text {"Expression: %s == %s within %.*g %s (over %.*g %s).",
                                  site.lhs,
                                  site.rhs,
                                  digits,
                                  distance,
                                  unit,
                                  digits,
                                  static_cast<double>(n),
                                  unit});
      return logger {false};
    }

    return check_failed(
    site,
    ::tts::text {"Expected: %s == %s but %s == %s within %.*g %s when  %.*g %s were expected.",
                 site.lhs,
                 site.rhs,
                 ::tts::as_text(a).data(),
                 ::tts::as_text(b).data(),
                 digits,
                 distance,
                 unit,
                 digits,
                 static_cast<double>(n),
                 unit});
  }

  template<typename A, typename B> logger check_ieee(A const& a, B const& b, check_site const& site)
  {
    if(::tts::ieee_check(a, b))
    {
      if(check_passed())
        check_report(site, ::tts::text {"Expression: %s == %s.", site.lhs, site.rhs});
      return logger {false};
    }

    return check_failed(site,
                        ::tts::text {"Expression: %s == %s is false because %s != %s.",
                                     site.lhs,
                                     site.rhs,
                                     ::tts::as_text(a).data(),
                                     ::tts::as_text(b).data()});
  }

  inline logger check_percent_tolerance(double n, check_site const& site)
  {
    return check_failed(
    site,
    ::tts::text {
    "Tolerance %.*g reads as a percentage: TTS 4 compares a ratio, divide it by a hundred.", 8, n});
  }
}

#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, MEASURE, PREC, FAILURE)                              \
  ::tts::_::check_precision<::tts::_::measure::MEASURE>(                                           \
  LHS, RHS, N, UNIT, PREC, TTS_CHECK_SITE(FAILURE, TTS_STRING(LHS), "", TTS_STRING(RHS), "")) /**/

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

  `L` and `R` must have the same type. A mismatch is a compile-time error: the distance would
  be measured after promotion to the common type, hence in the unit of that type rather than
  the one under test.

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
#define TTS_ABSOLUTE_EQUAL(L, R, N, ...) TTS_PRECISION(L, R, N, "unit", absolute, 8, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_RELATIVE_EQUAL
  @brief Checks if values are within a given relative distance.

  This comparison is performed by using the proper tts::relative_distance overload.

  `L` and `R` must have the same type. A mismatch is a compile-time error: the distance would
  be measured after promotion to the common type, hence in the unit of that type rather than
  the one under test.

  @param L, R Expressions to compare.
  @param N    Maximum relative distance accepted between `L` and `R`, as a ratio: `0.005` is
              half a percent. `abs(L-R) / max(abs(L), abs(R), 1)` is what gets compared to it.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/relative.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_RELATIVE_EQUAL(L, R, N, ...)
#else
#define TTS_RELATIVE_EQUAL(L, R, N, ...)                                                           \
  (                                                                                                \
  ::tts::_::reads_as_percent<decltype(L)>(N)                                                       \
  ? TTS_PERCENT_TOLERANCE_##__VA_ARGS__(N)                                                         \
  : TTS_PRECISION(L, R, N, "rel", relative, 8, __VA_ARGS__))

#define TTS_PERCENT_TOLERANCE_(N)         TTS_PERCENT_TOLERANCE_IMPL(N, TTS_FAIL)
#define TTS_PERCENT_TOLERANCE_REQUIRED(N) TTS_PERCENT_TOLERANCE_IMPL(N, TTS_FATAL)
#define TTS_PERCENT_TOLERANCE_IMPL(N, FAILURE)                                                     \
  ::tts::_::check_percent_tolerance(static_cast<double>(N),                                        \
                                    TTS_CHECK_SITE(FAILURE, "", "", "", "")) /**/
#endif

//======================================================================================================================
/**
  @def TTS_ULP_EQUAL
  @brief Checks if two values are within a given ULP distance

  This comparison is performed by using the proper tts::ulp_distance overload.

  `L` and `R` must have the same type. A mismatch is a compile-time error: the distance would
  be measured after promotion to the common type, hence in the unit of that type rather than
  the one under test.

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
#define TTS_ULP_EQUAL(L, R, N, ...) TTS_PRECISION(L, R, N, "ULP", ulp, 2, __VA_ARGS__)
#endif

#define TTS_DO_IEEE_EQUAL_IMPL(LHS, RHS, FAILURE)                                                  \
  ::tts::_::check_ieee(                                                                            \
  LHS, RHS, TTS_CHECK_SITE(FAILURE, TTS_STRING(LHS), "", TTS_STRING(RHS), "")) /**/

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
