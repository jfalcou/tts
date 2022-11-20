//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <tts/engine/comparators.hpp>
#include <tts/tools/as_string.hpp>
#include <tts/test/info.hpp>

#define TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                \
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
/**/

#define TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                          \
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
/**/

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                  \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                        \
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
  TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                  \
}(A,B)                                                                                              \
/**/

#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)          TTS_CEXPR_RELATION(LHS,RHS, eq , "==" , "!=", __VA_ARGS__)
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)      TTS_CEXPR_RELATION(LHS,RHS, neq, "!=" , "==", __VA_ARGS__)
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)           TTS_CEXPR_RELATION(LHS,RHS, lt , "<"  , ">=", __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)        TTS_CEXPR_RELATION(LHS,RHS, gt , ">"  , "<=", __VA_ARGS__)
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)     TTS_CEXPR_RELATION(LHS,RHS, le , "<=" , ">" , __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)  TTS_CEXPR_RELATION(LHS,RHS, ge , ">=" , "<=", __VA_ARGS__)

#define TTS_TYPED_RELATION(A, B, OP, T, F, ...)     TTS_TYPED_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_TYPED_RELATION_(A, B, OP, T, F)         TTS_TYPED_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_TYPED_RELATION_REQUIRED(A, B, OP, T, F) TTS_TYPED_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                            \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  using type_a = std::remove_cvref_t<decltype(a)>;                                                  \
  using type_b = std::remove_cvref_t<decltype(b)>;                                                  \
                                                                                                    \
  if constexpr( !tts::same_as<type_a, type_b> )                                                     \
  {                                                                                                 \
      FAILURE (   "Expression: "  << TTS_STRING(A) << " " T " " << TTS_STRING(B)                    \
              <<  " is false because: " << ::tts::typename_<type_a> << " is not "                   \
              << ::tts::typename_<type_b>                                                           \
              );                                                                                    \
      return ::tts::logger{};                                                                       \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                      \
  }                                                                                                 \
}(A,B)                                                                                              \
/**/

#define TTS_TYPED_EQUAL(LHS, RHS, ...)          TTS_TYPED_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)      TTS_TYPED_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)
#define TTS_TYPED_LESS(LHS, RHS, ...)           TTS_TYPED_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)
#define TTS_TYPED_GREATER(LHS, RHS, ...)        TTS_TYPED_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...)     TTS_TYPED_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)  TTS_TYPED_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)

#define TTS_TYPED_CEXPR_RELATION(A, B, OP, T, F, ...)     TTS_TYPED_CEXPR_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_TYPED_CEXPR_RELATION_(A, B, OP, T, F)         TTS_TYPED_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_TYPED_CEXPR_RELATION_REQUIRED(A, B, OP, T, F) TTS_TYPED_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                      \
[&](auto&& a, auto&& b)                                                                             \
{                                                                                                   \
  using type_a = std::remove_cvref_t<decltype(a)>;                                                  \
  using type_b = std::remove_cvref_t<decltype(b)>;                                                  \
                                                                                                    \
  if constexpr( !tts::same_as<type_a, type_b> )                                                     \
  {                                                                                                 \
      FAILURE (   "Expression: "  << TTS_STRING(A) << " " T " " << TTS_STRING(B)                    \
              <<  " is false because: " << ::tts::typename_<type_a> << " is not "                   \
              << ::tts::typename_<type_b>                                                           \
              );                                                                                    \
      return ::tts::logger{};                                                                       \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                \
  }                                                                                                 \
}(A,B)                                                                                              \
/**/

#define TTS_TYPED_CONSTEXPR_EQUAL(LHS, RHS, ...)          TTS_TYPED_CEXPR_RELATION(LHS,RHS, eq , "==" , "!=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)      TTS_TYPED_CEXPR_RELATION(LHS,RHS, neq, "!=" , "==", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS(LHS, RHS, ...)           TTS_TYPED_CEXPR_RELATION(LHS,RHS, lt , "<"  , ">=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER(LHS, RHS, ...)        TTS_TYPED_CEXPR_RELATION(LHS,RHS, gt , ">"  , "<=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)     TTS_TYPED_CEXPR_RELATION(LHS,RHS, le , "<=" , ">" , __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)  TTS_TYPED_CEXPR_RELATION(LHS,RHS, ge , ">=" , "<=", __
