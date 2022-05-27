//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/tools/types.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>

#define TTS_TYPE_IS(TYPE, REF, ...)     TTS_TYPE_IS_ ## __VA_ARGS__ (TYPE, REF)
#define TTS_TYPE_IS_(TYPE, REF)         TTS_TYPE_IS_IMPL(TYPE, REF,TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(TYPE, REF) TTS_TYPE_IS_IMPL(TYPE, REF,TTS_FATAL)

#define TTS_TYPE_IS_IMPL(TYPE, REF, FAILURE)                                                        \
[&]<typename T, typename R>(::tts::type<T>, ::tts::type<R>)                                         \
{                                                                                                   \
  if constexpr( std::is_same_v<T,R> )                                                               \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Type: "  << TTS_STRING(TTS_REMOVE_PARENS(TYPE)) << " is not the same as "            \
                        << TTS_STRING(TTS_REMOVE_PARENS(REF))  << " because "                       \
                        << ::tts::typename_<T> << " is not " << ::tts::typename_<R>                 \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(::tts::type<TTS_REMOVE_PARENS(TYPE)>{}, ::tts::type<TTS_REMOVE_PARENS(REF)>{})                    \
/**/

#define TTS_EXPR_IS(EXPR, TYPE, ...)     TTS_EXPR_IS_ ## __VA_ARGS__ (EXPR, TYPE)
#define TTS_EXPR_IS_(EXPR, TYPE)         TTS_EXPR_IS_IMPL(EXPR, TYPE,TTS_FAIL)
#define TTS_EXPR_IS_REQUIRED(EXPR, TYPE) TTS_EXPR_IS_IMPL(EXPR, TYPE,TTS_FATAL)

#define TTS_EXPR_IS_IMPL(EXPR, TYPE, FAILURE)                                                       \
[&]<typename T, typename R>(::tts::type<T>, ::tts::type<R>)                                         \
{                                                                                                   \
  if constexpr( std::is_same_v<T,R> )                                                               \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Type: "  << TTS_STRING(TTS_REMOVE_PARENS(EXPR))  << " is not the same as "         \
                          << TTS_STRING(TTS_REMOVE_PARENS(TYPE)) << " because "                     \
                          << ::tts::typename_<T> << " is not " << ::tts::typename_<R>               \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(::tts::type<decltype(TTS_REMOVE_PARENS(EXPR))>{}, ::tts::type<TTS_REMOVE_PARENS(TYPE)>{})         \
/**/

#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                             \
[&]( TTS_ARG(__VA_ARGS__) )                                                                           \
{                                                                                                     \
  if constexpr( requires TTS_REMOVE_PARENS(EXPR) )                                                    \
  {                                                                                                   \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                        \
  }                                                                                                   \
  else                                                                                                \
  {                                                                                                   \
    TTS_FAIL(     "Expression: " << TTS_STRING(TTS_REMOVE_PARENS(EXPR))                               \
              <<  " does not compile as expected."                                                    \
            );                                                                                        \
    return ::tts::logger{};                                                                           \
  }                                                                                                   \
}(__VA_ARGS__)                                                                                        \
/**/

#define TTS_EXPECT_COMPILES(...) TTS_VAL(TTS_EXPECT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__) )

#define TTS_EXPECT_NOT_COMPILES_IMPL(EXPR, ...)                                                       \
[&]( TTS_ARG(__VA_ARGS__) )                                                                           \
{                                                                                                     \
  if constexpr( !(requires TTS_REMOVE_PARENS(EXPR)) )                                                 \
  {                                                                                                   \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                        \
  }                                                                                                   \
  else                                                                                                \
  {                                                                                                   \
    TTS_FAIL("Expression: " << TTS_STRING(TTS_REMOVE_PARENS(EXPR)) << " compiles unexpectedly." );    \
    return ::tts::logger{};                                                                           \
  }                                                                                                   \
}(__VA_ARGS__)                                                                                        \
/**/

#define TTS_EXPECT_NOT_COMPILES(...) TTS_VAL(TTS_EXPECT_NOT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
