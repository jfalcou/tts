//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/precision.hpp>
#include <tts/engine/logger.hpp>

#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, PREC,FAILURE)                                   \
[&](auto lhs, auto rhs)                                                                             \
{                                                                                                   \
  auto r = FUNC (lhs,rhs);                                                                          \
                                                                                                    \
  if(r <= N)                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)                          \
                            << " but "                                                              \
                            << ::tts::as_string(lhs) << " == " << ::tts::as_string(rhs)             \
                            << " within " << std::setprecision(PREC) << std::fixed                  \
                            << r << std::defaultfloat                                               \
                            << " " << UNIT << " when "                                              \
                            << std::setprecision(PREC) <<  std::fixed                               \
                            << N << std::defaultfloat                                               \
                            << " " << UNIT << " was expected."                                      \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}(LHS,RHS)                                                                                          \
/**/

#define TTS_PRECISION(L,R,N,U,F,P,...)      TTS_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F,P)
#define TTS_PRECISION_(L,R,N,U,F,P)         TTS_PRECISION_IMPL(L,R,N,U,F,P,TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L,R,N,U,F,P) TTS_PRECISION_IMPL(L,R,N,U,F,P,TTS_FATAL)

#define TTS_ABSOLUTE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"unit", ::tts::absolute_distance, 8, __VA_ARGS__ )
#define TTS_RELATIVE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"%"   , ::tts::relative_distance, 8, __VA_ARGS__ )
#define TTS_ULP_EQUAL(L,R,N,...)      TTS_PRECISION(L,R,N,"ULP" , ::tts::ulp_distance     , 2, __VA_ARGS__ )
#define TTS_IEEE_EQUAL(L,R,...)       TTS_ULP_EQUAL(L, R, 0, __VA_ARGS__ )
