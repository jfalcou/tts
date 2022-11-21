//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>
#include <tts/test/info.hpp>

#define TTS_EXPECT(EXPR, ...)     TTS_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL((EXPR),TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL((EXPR),TTS_FATAL)

#define TTS_EXPECT_IMPL(EXPR,FAILURE)                                                               \
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( expr )                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(TTS_REMOVE_PARENS(EXPR)) << " evaluates to false." );   \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(EXPR)                                                                                             \
/**/

#define TTS_EXPECT_NOT(EXPR, ...)       TTS_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_NOT_(EXPR)           TTS_EXPECT_NOT_IMPL(EXPR,TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR)   TTS_EXPECT_NOT_IMPL(EXPR,TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                           \
[&](auto&& expr)                                                                                    \
{                                                                                                   \
  if( !expr )                                                                                       \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: "  << TTS_STRING(EXPR) << " evaluates to true." );                       \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(EXPR)                                                                                             \
/**/

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
::tts::logger{::tts::global_logger_status}                                                          \
/**/

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
::tts::logger{::tts::global_logger_status}                                                          \
/**/
