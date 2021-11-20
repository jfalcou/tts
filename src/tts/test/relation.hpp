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

#define TTS_RELATION(A, B, OP, T, F, ...)    TTS_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                  \
[&]()                                                                                               \
{                                                                                                   \
  if( ::tts::detail::OP(A,B) )                                                                      \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Expression: "  << TTS_STRING(A) << " " T " " << TTS_STRING(B)                      \
            <<  " is false because: " << ::tts::as_string(A) << " " F " " << ::tts::as_string(B)    \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()                                                                                                 \
/**/

#define TTS_EQUAL(LHS, RHS, ...)          TTS_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)
#define TTS_NOT_EQUAL(LHS, RHS, ...)      TTS_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)
#define TTS_LESS(LHS, RHS, ...)           TTS_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)
#define TTS_GREATER(LHS, RHS, ...)        TTS_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)
#define TTS_LESS_EQUAL(LHS, RHS, ...)     TTS_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)
#define TTS_GREATER_EQUAL(LHS, RHS, ...)  TTS_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)

#define TTS_CONSTEXPR_RELATION(A, B, OP, T, F)                                                      \
{                                                                                                   \
  static_assert ( std::bool_constant<::tts::detail::OP(A,B)>::value                                 \
                , "[TTS] - ** FAILURE** : " TTS_STRING(A) " " T " " TTS_STRING(B) " is false."      \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}                                                                                                   \
/**/

#define TTS_CONSTEXPR_EQUAL(LHS, RHS)          TTS_CONSTEXPR_RELATION(LHS,RHS, eq , "==" , "!=")
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS)      TTS_CONSTEXPR_RELATION(LHS,RHS, neq, "!=" , "==")
#define TTS_CONSTEXPR_LESS(LHS, RHS)           TTS_CONSTEXPR_RELATION(LHS,RHS, lt , "<"  , ">=")
#define TTS_CONSTEXPR_GREATER(LHS, RHS)        TTS_CONSTEXPR_RELATION(LHS,RHS, gt , ">"  , "<=")
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS)     TTS_CONSTEXPR_RELATION(LHS,RHS, le , "<=" , ">" )
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS)  TTS_CONSTEXPR_RELATION(LHS,RHS, ge , ">=" , "<=")
