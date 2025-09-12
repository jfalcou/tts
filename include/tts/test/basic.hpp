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
  @def TTS_EXPECT
  @brief Check if a given expression evaluates to true

  @param EXPR Expression to evaluate and compare to `true`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expectation can be met" )
  {
    int a = 42, b = 69;

    TTS_EXPECT(a <  b);
    TTS_EXPECT(b >  a);
    TTS_EXPECT(a != b);

    // This test would cause the program to stop and not run the remaining tests
    // TTS_EXPECT(a == b, REQUIRED);

    TTS_EXPECT(a <= b);
    TTS_EXPECT(b >= a);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_EXPECT(EXPR, ...)     TTS_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL((EXPR),TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR),TTS_FATAL)

#define TTS_EXPECT_IMPL(EXPR,FAILURE)                                                               \
[&](auto&& local_tts_expr)                                                                          \
{                                                                                                   \
  if( local_tts_expr )                                                                              \
  {                                                                                                 \
    TTS_PASS( "Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );           \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );          \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
}(EXPR)                                                                                             \
/**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT
  @brief Check if a given expression evaluates to false

  @param EXPR Expression to evaluate and compare to `false`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expectation can be met" )
  {
    int a = 42, b = 69;

    TTS_EXPECT_NOT(a == b);
    TTS_EXPECT_NOT(b < a);

    // This test would cause the program to stop and not run the remaining tests
    // TTS_EXPECT_NOT(a != b, REQUIRED);

    TTS_EXPECT_NOT(a >= b);
    TTS_EXPECT_NOT(a <= b);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_EXPECT_NOT(EXPR, ...)       TTS_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_NOT_(EXPR)           TTS_EXPECT_NOT_IMPL(EXPR,TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR)   TTS_EXPECT_NOT_IMPL(EXPR,TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                           \
[&](auto&& local_tts_expr)                                                                          \
{                                                                                                   \
  if( !local_tts_expr )                                                                             \
  {                                                                                                 \
    TTS_PASS( "Expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );          \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );           \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
}(EXPR)                                                                                             \
/**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT
  @brief Check if a given expression evaluates to true at compile-time

  @param EXPR Expression to evaluate and compare to `true`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expectation can be met" )
  {
    constexpr int a = 42, b = 69;

    TTS_CONSTEXPR_EXPECT(a != b);
    TTS_CONSTEXPR_EXPECT(a <  b);
    TTS_CONSTEXPR_EXPECT(a <= b);
    TTS_CONSTEXPR_EXPECT(b >  a);
    TTS_CONSTEXPR_EXPECT(b >= a);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_EXPECT(EXPR, ...) TTS_CEXPR_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CEXPR_EXPECT_(EXPR)         TTS_CEXPR_EXPECT_IMPL(EXPR,TTS_FAIL)
#define TTS_CEXPR_EXPECT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_IMPL(EXPR,TTS_FATAL)

#define TTS_CEXPR_EXPECT_IMPL(EXPR,FAILURE)                                                         \
do                                                                                                  \
{                                                                                                   \
  constexpr auto local_tts_expr = EXPR;                                                             \
  if constexpr( local_tts_expr )                                                                    \
  {                                                                                                 \
    TTS_PASS( "Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );  \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Constant expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) ); \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
}while(0)                                                                                           \
/**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EXPECT_NOT
  @brief Check if a given expression evaluates to false at compile-time

  @param EXPR Expression to evaluate and compare to `false`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that counter-expectation can be met" )
  {
    constexpr int a = 42, b = 69;

    TTS_CONSTEXPR_EXPECT_NOT(a == b);
    TTS_CONSTEXPR_EXPECT_NOT(a >  b);
    TTS_CONSTEXPR_EXPECT_NOT(a >= b);
    TTS_CONSTEXPR_EXPECT_NOT(b <  a);
    TTS_CONSTEXPR_EXPECT_NOT(b <= a);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...) TTS_CEXPR_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CEXPR_EXPECT_NOT_(EXPR)         TTS_CEXPR_EXPECT_NOT_IMPL(EXPR,TTS_FAIL)
#define TTS_CEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CEXPR_EXPECT_NOT_IMPL(EXPR,TTS_FATAL)

#define TTS_CEXPR_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                     \
do                                                                                                  \
{                                                                                                   \
  constexpr auto local_tts_expr = EXPR;                                                             \
  if constexpr( !local_tts_expr )                                                                   \
  {                                                                                                 \
    TTS_PASS( "Constant expression: %s evaluates to false.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) ); \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Constant expression: %s evaluates to true.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)) );  \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
}while(0)                                                                                           \
/**/
