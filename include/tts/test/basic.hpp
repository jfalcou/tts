//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>
#include <tts/engine/info.hpp>
#include <tts/engine/check.hpp>
#include <tts/engine/context.hpp>

namespace tts::_
{
  template<typename Context>
  constexpr auto check_expect(Context&          ctx,
                              bool              value,
                              bool              expected,
                              check_site const& site,
                              char const*       kind = "Expression")
  {
    if(value == expected)
      return ctx.pass(site,
                      [ &kind, &site, &expected ]
                      {
                        return ::tts::text {
                        "%s: %s evaluates to %s.", kind, site.lhs, expected ? "true" : "false"};
                      });

    return ctx.fail(
    site,
    [ &kind, &site, &value ]
    { return ::tts::text {"%s: %s evaluates to %s.", kind, site.lhs, value ? "true" : "false"}; });
  }
}

//======================================================================================================================
/**
  @defgroup test-basic Basic Tests Macros
  @brief Macros for performing direct true/false checks over predicate-like expressions.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @name Predicate Checks
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_EXPECT
  @brief Checks that an expression is `true`.

  @param EXPR Expression to check, converted to `bool`.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/expect.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT(EXPR, ...)
#else
#define TTS_EXPECT(EXPR, ...) TTS_EXPECT_##__VA_ARGS__(EXPR)
#endif

#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL((EXPR), TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR), TTS_FATAL)

#define TTS_EXPECT_IMPL(EXPR, FAILURE)                                                             \
  ::tts::_::check_expect(                                                                          \
  local_tts_ctx(),                                                                                 \
  static_cast<bool>(EXPR),                                                                         \
  true,                                                                                            \
  TTS_CHECK_SITE(FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", "")) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT
  @brief Checks that an expression is `false`.

  @param EXPR Expression to check, converted to `bool`.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/expect_not.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_NOT(EXPR, ...)
#else
#define TTS_EXPECT_NOT(EXPR, ...) TTS_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif

#define TTS_EXPECT_NOT_(EXPR)         TTS_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR) TTS_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                         \
  ::tts::_::check_expect(                                                                          \
  local_tts_ctx(),                                                                                 \
  static_cast<bool>(EXPR),                                                                         \
  false,                                                                                           \
  TTS_CHECK_SITE(FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", "")) /**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT
  @brief Checks at compile time that a constant expression is `true`.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param EXPR Constant expression to check, converted to `bool`.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_expect.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EXPECT(EXPR, ...)
#else
#define TTS_CONSTEXPR_EXPECT(EXPR, ...) TTS_CEXPR_EXPECT_##__VA_ARGS__(EXPR)
#endif

#define TTS_CEXPR_EXPECT_(EXPR)         TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FATAL)

#define TTS_CEXPR_EXPECT_IMPL(EXPR, FAILURE)                                                       \
  ::tts::_::check_expect(local_tts_ctx(),                                                          \
                         std::bool_constant<static_cast<bool>(EXPR)>::value,                       \
                         true,                                                                     \
                         TTS_CHECK_SITE(FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""), \
                         "Constant expression")
/**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT_NOT
  @brief Checks at compile time that a constant expression is `false`.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param EXPR Constant expression to check, converted to `bool`.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_expect_not.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...)
#else
#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...) TTS_CEXPR_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif

#define TTS_CEXPR_EXPECT_NOT_(EXPR)         TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)

#define TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                   \
  ::tts::_::check_expect(local_tts_ctx(),                                                          \
                         std::bool_constant<static_cast<bool>(EXPR)>::value,                       \
                         false,                                                                    \
                         TTS_CHECK_SITE(FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""), \
                         "Constant expression") /**/

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
