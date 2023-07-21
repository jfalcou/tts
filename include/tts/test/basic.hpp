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
#include <tts/test/info.hpp>

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
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( expr )                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(TTS_REMOVE_PARENS(EXPR)) << " evaluates to false." );   \
    return ::tts::detail::logger{};                                                                 \
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
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( !expr )                                                                                       \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(EXPR) << " evaluates to true." );                       \
    return ::tts::detail::logger{};                                                                 \
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
::tts::global_logger_status = false;                                                                \
do                                                                                                  \
{                                                                                                   \
  constexpr auto result_tts = EXPR;                                                                 \
  if( result_tts )                                                                                  \
  {                                                                                                 \
    ::tts::global_runtime.pass();                                                                   \
    ::tts::global_logger_status = false;                                                            \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(EXPR) << " evaluates to false." );                      \
    ::tts::global_logger_status = true;                                                             \
  }                                                                                                 \
}while(0);                                                                                          \
::tts::detail::logger{::tts::global_logger_status}                                                  \
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
::tts::global_logger_status = false;                                                                \
do                                                                                                  \
{                                                                                                   \
  constexpr auto result_tts = EXPR;                                                                 \
  if( !result_tts )                                                                                 \
  {                                                                                                 \
    ::tts::global_runtime.pass();                                                                   \
    ::tts::global_logger_status = false;                                                            \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(EXPR) << " evaluates to true." );                       \
    ::tts::global_logger_status = true;                                                             \
  }                                                                                                 \
}while(0);                                                                                          \
::tts::detail::logger{::tts::global_logger_status}                                                  \
/**/
