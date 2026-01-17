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
  @brief Check if a given expression evaluates to true

  @param EXPR Expression to evaluate and compare to `true`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/expect.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_EXPECT(EXPR, ...)
#else
#  define TTS_EXPECT(EXPR, ...) TTS_EXPECT_##__VA_ARGS__(EXPR)
#endif

#define TTS_EXPECT_(EXPR) TTS_EXPECT_IMPL((EXPR), TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR), TTS_FATAL)

#define TTS_EXPECT_IMPL(EXPR, FAILURE)                                                             \
  [ & ](auto &&local_tts_expr)                                                                     \
  {                                                                                                \
    if(local_tts_expr)                                                                             \
    {                                                                                              \
      TTS_PASS("Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));          \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));          \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(EXPR) /**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT
  @brief Check if a given expression evaluates to false

  @param EXPR Expression to evaluate and compare to `false`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/expect_not.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_EXPECT_NOT(EXPR, ...)
#else
#  define TTS_EXPECT_NOT(EXPR, ...) TTS_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif

#define TTS_EXPECT_NOT_(EXPR) TTS_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR) TTS_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                         \
  [ & ](auto &&local_tts_expr)                                                                     \
  {                                                                                                \
    if(!local_tts_expr)                                                                            \
    {                                                                                              \
      TTS_PASS("Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));         \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }(EXPR) /**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT
  @brief Check if a given expression evaluates to true at compile-time

  @param EXPR Expression to evaluate and compare to `true`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/cexpr_expect.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_CONSTEXPR_EXPECT(EXPR, ...)
#else
#  define TTS_CONSTEXPR_EXPECT(EXPR, ...) TTS_CEXPR_EXPECT_##__VA_ARGS__(EXPR)
#endif

#define TTS_CEXPR_EXPECT_(EXPR) TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_IMPL(EXPR, TTS_FATAL)

#define TTS_CEXPR_EXPECT_IMPL(EXPR, FAILURE)                                                       \
  do {                                                                                             \
    constexpr auto local_tts_expr = EXPR;                                                          \
    if constexpr(local_tts_expr)                                                                   \
    {                                                                                              \
      TTS_PASS("Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR))); \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Constant expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR))); \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  } while(0) /**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT_NOT
  @brief Check if a given expression evaluates to false at compile-time

  @param EXPR Expression to evaluate and compare to `false`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}
  @snippet doc/cexpr_expect_not.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...)
#else
#  define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...) TTS_CEXPR_EXPECT_NOT_##__VA_ARGS__(EXPR)
#endif

#define TTS_CEXPR_EXPECT_NOT_(EXPR) TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FAIL)
#define TTS_CEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, TTS_FATAL)

#define TTS_CEXPR_EXPECT_NOT_IMPL(EXPR, FAILURE)                                                   \
  do {                                                                                             \
    constexpr auto local_tts_expr = EXPR;                                                          \
    if constexpr(!local_tts_expr)                                                                  \
    {                                                                                              \
      TTS_PASS("Constant expression: %s evaluates to false.",                                      \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)));                                               \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));  \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  } while(0) /**/

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
