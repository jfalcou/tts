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
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>

namespace tts::_
{
  template<typename T, typename R> logger check_type(check_site const& site, bool expression)
  {
    if constexpr(std::is_same_v<T, R>)
    {
      if(check_passed())
        check_report(site,
                     expression
                     ? ::tts::text {"Expression: '%s' is of type '%s'.", site.lhs, site.rhs}
                     : ::tts::text {"Type: '%s' is '%s'.", site.lhs, site.rhs});
      return logger {false};
    }
    else
    {
      char const* format = expression
                           ? "Expression: '%s' is not of type '%s' because '%.*s' is not '%.*s'."
                           : "Type: '%s' is not the same as '%s' because '%.*s' is not '%.*s'.";
      return check_failed(site,
                          ::tts::text {format,
                                       site.lhs,
                                       site.rhs,
                                       ::tts::typename_<T>.size(),
                                       ::tts::typename_<T>.data(),
                                       ::tts::typename_<R>.size(),
                                       ::tts::typename_<R>.data()});
    }
  }

  inline logger check_compiles(bool compiles, bool expected, check_site const& site)
  {
    if(compiles == expected)
    {
      if(check_passed())
        check_report(site,
                     ::tts::text {compiles ? "Expression: '%s' compiles as expected."
                                           : "Expression: '%s' does not compile as expected.",
                                  site.lhs});
      return logger {false};
    }

    return check_failed(site,
                        ::tts::text {compiles ? "Expression: '%s' compiles unexpectedly."
                                              : "Expression: '%s' does not compile as expected.",
                                     site.lhs});
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
  @brief Checks if two types satisfy `std::is_same_v<Type,Target> == true`.

  @param TYPE Type to compare.
  @param REF  Expected type.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  TTS_CHECK_SITE(                                                                                  \
  FAILURE, TTS_STRING(TTS_REMOVE_PARENS(TYPE)), "", TTS_STRING(TTS_REMOVE_PARENS(REF)), ""),       \
  false) /**/

//======================================================================================================================
/**
  @def TTS_EXPR_IS
  @brief Checks if an Expression evaluates to a value of a given Type.

  @param EXPR Expression to evaluate.
  @param TYPE Expected type.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  TTS_CHECK_SITE(                                                                                  \
  FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", TTS_STRING(TTS_REMOVE_PARENS(TYPE)), ""),      \
  true) /**/

#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                        \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  [ & ](TTS_ARG(__VA_ARGS__))                                                                      \
  {                                                                                                \
    return ::tts::_::check_compiles(                                                               \
    requires TTS_REMOVE_PARENS(EXPR),                                                              \
    true,                                                                                          \
    TTS_CHECK_SITE(TTS_FAIL, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""));                    \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will compile properly in a SFINAE
context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.

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
    requires TTS_REMOVE_PARENS(EXPR),                                                              \
    false,                                                                                         \
    TTS_CHECK_SITE(TTS_FAIL, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""));                    \
    TTS_DISABLE_WARNING_POP                                                                        \
  }(__VA_ARGS__) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will not compile properly in a
SFINAE context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.

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
