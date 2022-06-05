//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/engine/comparators.hpp>
#include <tts/tools/as_string.hpp>
#include <tts/test/info.hpp>

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                  \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  if( ::tts::detail::OP(a,b) )                                                                      \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Expression: "  << TTS_STRING(A) << " " T " " << TTS_STRING(B)                      \
            <<  " is false because: " << ::tts::as_string(a) << " " F " " << ::tts::as_string(b)    \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(A,B)                                                                                              \
/**/

#define TTS_EQUAL(LHS, RHS, ...)          TTS_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)
#define TTS_NOT_EQUAL(LHS, RHS, ...)      TTS_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)
#define TTS_LESS(LHS, RHS, ...)           TTS_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)
#define TTS_GREATER(LHS, RHS, ...)        TTS_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)
#define TTS_LESS_EQUAL(LHS, RHS, ...)     TTS_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)
#define TTS_GREATER_EQUAL(LHS, RHS, ...)  TTS_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)

#define TTS_CEXPR_RELATION(A, B, OP, T, F, ...)     TTS_CEXPR_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_CEXPR_RELATION_(A, B, OP, T, F)         TTS_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_CEXPR_RELATION_REQUIRED(A, B, OP, T, F) TTS_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                            \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  using result_tts = std::bool_constant<::tts::detail::OP(A,B)>;                                    \
  if constexpr( result_tts::value )                                                                 \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Expression: "  << TTS_STRING(A) << " " << T << " " << TTS_STRING(B)                \
            <<  " is false because: "                                                               \
            << ::tts::as_string(a) << " " << F << " " << ::tts::as_string(b)                        \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(A,B)                                                                                              \
/**/

#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)          TTS_CEXPR_RELATION(LHS,RHS, eq , "==" , "!=", __VA_ARGS__)
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)      TTS_CEXPR_RELATION(LHS,RHS, neq, "!=" , "==", __VA_ARGS__)
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)           TTS_CEXPR_RELATION(LHS,RHS, lt , "<"  , ">=", __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)        TTS_CEXPR_RELATION(LHS,RHS, gt , ">"  , "<=", __VA_ARGS__)
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)     TTS_CEXPR_RELATION(LHS,RHS, le , "<=" , ">" , __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)  TTS_CEXPR_RELATION(LHS,RHS, ge , ">=" , "<=", __VA_ARGS__)
