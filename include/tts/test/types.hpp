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
#include <tts/engine/check.hpp>
#include <tts/engine/context.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>

namespace tts::_
{
  template<typename T, typename R, typename Context>
  constexpr auto check_type(Context& ctx, check_site const& site, bool expression)
  {
    if constexpr(std::is_same_v<T, R>)
      return ctx.pass(
      site,
      [ &site, &expression ]
      {
        return expression ? ::tts::text {"Expression: '%s' is of type '%s'.", site.lhs, site.rhs}
                          : ::tts::text {"Type: '%s' is '%s'.", site.lhs, site.rhs};
      });
    else
      return ctx.fail(site,
                      [ &site, &expression ]
                      {
                        char const* format =
                        expression
                        ? "Expression: '%s' is not of type '%s' because '%.*s' is not '%.*s'."
                        : "Type: '%s' is not the same as '%s' because '%.*s' is not '%.*s'.";
                        return ::tts::text {format,
                                            site.lhs,
                                            site.rhs,
                                            ::tts::typename_<T>.size(),
                                            ::tts::typename_<T>.data(),
                                            ::tts::typename_<R>.size(),
                                            ::tts::typename_<R>.data()};
                      });
  }

  template<typename Context>
  constexpr auto check_compiles(Context& ctx, bool compiles, bool expected, check_site const& site)
  {
    if(compiles == expected)
      return ctx.pass(site,
                      [ &site, &compiles ]
                      {
                        return ::tts::text {compiles
                                            ? "Expression: '%s' compiles as expected."
                                            : "Expression: '%s' does not compile as expected.",
                                            site.lhs};
                      });

    return ctx.fail(site,
                    [ &site, &compiles ]
                    {
                      return ::tts::text {compiles
                                          ? "Expression: '%s' compiles unexpectedly."
                                          : "Expression: '%s' does not compile as expected.",
                                          site.lhs};
                    });
  }
}

//======================================================================================================================
/**
  @defgroup test-types Type Tests Macros
  @brief Macros for performing checks on types and their properties.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_TYPE_IS
  @brief Checks that two types are the same.

  @param TYPE Type to check.
  @param REF  Type expected.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/type_is.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPE_IS(TYPE, REF, ...)
#else
#define TTS_TYPE_IS(TYPE, REF, ...) TTS_TYPE_IS_##__VA_ARGS__(TYPE, REF)
#endif

#define TTS_TYPE_IS_(TYPE, REF)         TTS_TYPE_IS_IMPL(TYPE, REF, TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(TYPE, REF) TTS_TYPE_IS_IMPL(TYPE, REF, TTS_FATAL)

#define TTS_TYPE_IS_IMPL(TYPE, REF, FAILURE)                                                       \
  ::tts::_::check_type<TTS_REMOVE_PARENS(TYPE), TTS_REMOVE_PARENS(REF)>(                           \
  local_tts_ctx(),                                                                                 \
  TTS_CHECK_SITE(                                                                                  \
  FAILURE, TTS_STRING(TTS_REMOVE_PARENS(TYPE)), "", TTS_STRING(TTS_REMOVE_PARENS(REF)), ""),       \
  false) /**/

//======================================================================================================================
/**
  @def TTS_EXPR_IS
  @brief Checks that an expression has the type `TYPE`.

  Compares `decltype(EXPR)` to `TYPE`, so an expression that names a variable gives its declared
  type.

  @param EXPR Expression whose type is checked.
  @param TYPE Type expected.
  @param ...  `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/expr_is.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPR_IS(EXPR, TYPE, ...)
#else
#define TTS_EXPR_IS(EXPR, TYPE, ...) TTS_EXPR_IS_##__VA_ARGS__(EXPR, TYPE)
#endif

#define TTS_EXPR_IS_(EXPR, TYPE)         TTS_EXPR_IS_IMPL(EXPR, TYPE, TTS_FAIL)
#define TTS_EXPR_IS_REQUIRED(EXPR, TYPE) TTS_EXPR_IS_IMPL(EXPR, TYPE, TTS_FATAL)

#define TTS_EXPR_IS_IMPL(EXPR, TYPE, FAILURE)                                                      \
  ::tts::_::check_type<decltype(TTS_REMOVE_PARENS(EXPR)), TTS_REMOVE_PARENS(TYPE)>(                \
  local_tts_ctx(),                                                                                 \
  TTS_CHECK_SITE(                                                                                  \
  FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", TTS_STRING(TTS_REMOVE_PARENS(TYPE)), ""),      \
  true) /**/

#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                        \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  [ & ](TTS_ARG(__VA_ARGS__))                                                                      \
  {                                                                                                \
    return ::tts::_::check_compiles(                                                               \
    local_tts_ctx(),                                                                               \
    requires TTS_REMOVE_PARENS(EXPR),                                                              \
    true,                                                                                          \
    TTS_CHECK_SITE(TTS_FAIL, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""));                    \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_COMPILES
  @brief Checks that an expression on the given symbols compiles.

  Checks the expression in a `requires` expression over the symbols, so a failure to compile is
  reported as a failed check instead of a compiler error.

  @param Symbols    Names the expression uses.
  @param Expression Expression to check, between braces.

  @groupheader{Example}
  @snippet doc/expect_compiles.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_COMPILES(Symbols, Expression, ...)
#else
#define TTS_EXPECT_COMPILES(...) TTS_VAL(TTS_EXPECT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
#endif

#define TTS_EXPECT_NOT_COMPILES_IMPL(EXPR, ...)                                                    \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  [ & ](TTS_ARG(__VA_ARGS__))                                                                      \
  {                                                                                                \
    return ::tts::_::check_compiles(                                                               \
    local_tts_ctx(),                                                                               \
    requires TTS_REMOVE_PARENS(EXPR),                                                              \
    false,                                                                                         \
    TTS_CHECK_SITE(TTS_FAIL, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""));                    \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT_COMPILES
  @brief Checks that an expression on the given symbols does not compile.

  Checks the expression in a `requires` expression over the symbols, so a failure to compile is
  reported as a failed check instead of a compiler error.

  @param Symbols    Names the expression uses.
  @param Expression Expression to check, between braces.

  @groupheader{Example}
  @snippet doc/expect_not_compiles.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_NOT_COMPILES(Symbols..., Expression, ...)
#else
#define TTS_EXPECT_NOT_COMPILES(...) TTS_VAL(TTS_EXPECT_NOT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
#endif

//======================================================================================================================
/// @}
//======================================================================================================================
