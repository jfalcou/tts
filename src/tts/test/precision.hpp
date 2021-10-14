//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/test/decomposer.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>
#include <tts/engine/precision.hpp>

namespace tts
{
  template<typename T, typename U> struct precision_result
  {
    std::string lhs,rhs;
    std::string lhs_val,rhs_val;
    T           value;
    U           maxi;
    bool        status;
    explicit operator bool() const { return status; }
  };
}
#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, FAILURE)                                        \
::tts::logger{}.check                                                                               \
( []<typename TTS_MAXI>(auto eval_a, auto eval_b, TTS_MAXI maxi)                                    \
  {                                                                                                 \
    auto r = FUNC(eval_a,eval_b);                                                                   \
    return  ::tts::precision_result<decltype(r),TTS_MAXI>                                           \
            { TTS_STRING(LHS), TTS_STRING(RHS), ::tts::as_string(eval_a), ::tts::as_string(eval_b)  \
            , r, maxi, r <= maxi                                                                    \
            };                                                                                      \
  }(LHS,RHS,N)                                                                                      \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    auto& fmt_n = res.maxi  < 1000  ? std::defaultfloat : std::scientific;                          \
    auto& fmt_r = res.value < 1000  ? std::defaultfloat : std::scientific;                          \
    TTS_PASS( ::tts::green  << res.lhs << " == " << res.rhs << tts::reset                           \
                            << " evaluates as " << ::tts::green                                     \
                            << res.lhs_val << " == " << res.rhs_val                                 \
                            << " within " << std::setprecision(2) << fmt_r                          \
                            << ::tts::green  << res.value << ::tts::reset << std::defaultfloat      \
                            << " " << UNIT << ::tts::reset << " when "                              \
                            << std::setprecision(2) << fmt_n                                        \
                            << ::tts::green  << res.maxi << ::tts::reset << std::defaultfloat       \
                            << " " << UNIT << " was expected."                                      \
                            );                                                                      \
    return false;                                                                                   \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    auto& fmt_n = res.maxi  < 1000  ? std::defaultfloat : std::scientific;                          \
    auto& fmt_r = res.value < 1000  ? std::defaultfloat : std::scientific;                          \
    FAILURE ( "Expected: "  << ::tts::green << res.lhs << " == " << res.rhs                         \
                            << tts::reset << " but " << ::tts::red                                  \
                            << res.lhs_val << " == " << res.rhs_val                                 \
                            << " within " << std::setprecision(2) << fmt_r                          \
                            << ::tts::red  << res.value << ::tts::reset << std::defaultfloat        \
                            << " " << UNIT << ::tts::reset << " when "                              \
                            << std::setprecision(2) << fmt_n                                        \
                            << ::tts::green  << res.maxi << ::tts::reset << std::defaultfloat       \
                            << " " << UNIT << " was expected."                                      \
            );                                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_PRECISION(L,R,N,U,F, ...)     TTS_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F)
#define TTS_PRECISION_(L,R,N,U,F)         TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L,R,N,U,F) TTS_PRECISION_IMPL(L,R,N,U,F,TTS_FATAL)

#define TTS_ABSOLUTE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"unit", ::tts::absolute_distance, __VA_ARGS__ )
#define TTS_RELATIVE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"%"   , ::tts::relative_distance, __VA_ARGS__ )
#define TTS_ULP_EQUAL(L,R,N,...)      TTS_PRECISION(L,R,N,"ULP" , ::tts::ulp_distance     , __VA_ARGS__ )
#define TTS_IEEE_EQUAL(L,R,...)       TTS_ULP_EQUAL(L, R, 0, __VA_ARGS__ )
