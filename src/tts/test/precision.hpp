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

#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, FAILURE)                                        \
[&]()                                                                                               \
{                                                                                                   \
  auto eval_a = (LHS);                                                                              \
  auto eval_b = (RHS);                                                                              \
  auto r      = FUNC (eval_a,eval_b);                                                               \
  auto& fmt_n = N<1000 ? std::defaultfloat : std::scientific;                                       \
  auto& fmt_r = r<1000 ? std::defaultfloat : std::scientific;                                       \
                                                                                                    \
  if(r <= N)                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)                          \
                            << " but "                                                              \
                            << ::tts::as_string(eval_a) << " == " << ::tts::as_string(eval_b)       \
                            << " within " << std::setprecision(2) << fmt_r                          \
                            << r << std::defaultfloat                                               \
                            << " " << UNIT << " when "                                              \
                            << std::setprecision(2) << fmt_n                                        \
                            << N << std::defaultfloat                                               \
                            << " " << UNIT << " was expected."                                      \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()
/**/

#define TTS_PRECISION(L,R,N,U,F, ...)     TTS_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F)
#define TTS_PRECISION_(L,R,N,U,F)         TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L,R,N,U,F) TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FATAL)

#define TTS_ABSOLUTE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"unit", ::tts::absolute_distance, __VA_ARGS__ )
#define TTS_RELATIVE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"%"   , ::tts::relative_distance, __VA_ARGS__ )
#define TTS_ULP_EQUAL(L,R,N,...)      TTS_PRECISION(L,R,N,"ULP" , ::tts::ulp_distance     , __VA_ARGS__ )
#define TTS_IEEE_EQUAL(L,R,...)       TTS_ULP_EQUAL(L, R, 0, __VA_ARGS__ )
