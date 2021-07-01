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

namespace tts::detail
{
  template<typename It1, typename It2, typename Func>
  std::pair<It1, It2> mismatch(It1 first1, It1 last1, It2 first2, Func p)
  {
    while (first1 != last1 && p(*first1, *first2))
      ++first1, ++first2;
    return std::make_pair(first1, first2);
  }
}

#define TTS_ALL_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, FAILURE)                                  \
[&]()                                                                                             \
{                                                                                                 \
  if( std::size(LHS) == std::size(RHS) )                                                          \
  {                                                                                               \
    auto found = ::tts::detail::mismatch( std::begin(LHS), std::end(LHS), std::begin(RHS)         \
                                        , [](auto l, auto r) { return ::tts::FUNC(l,r) <= N; }    \
                                        );                                                        \
    auto distance = std::end(LHS)-found.first;                                                    \
    auto& fmt_n = N<1000 ? std::defaultfloat : std::scientific;                                   \
                                                                                                  \
    if( distance == 0)                                                                            \
    {                                                                                             \
      TTS_PASS( ::tts::green  << TTS_STRING(LHS) << " == " << TTS_STRING(RHS) << tts::reset       \
                              << " evaluates within " << std::setprecision(2) << fmt_n            \
                              << ::tts::green  << N << ::tts::reset << std::defaultfloat          \
                              << " " << UNIT << " for all values."                                \
                              );                                                                  \
      return ::tts::logger{false};                                                                \
    }                                                                                             \
    else                                                                                          \
    {                                                                                             \
      FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)     \
                              << ::tts::reset << " but value at index " << ::tts::yellow          \
                              << distance     << ::tts::reset << " is " << ::tts::red             \
                              << ::tts::as_string(*found.first) << ::tts::reset                   \
                              << " instead of " << ::tts::red                                     \
                              << ::tts::as_string(*found.second)<< ::tts::reset                   \
                              << " within " << ::tts::red                                         \
                              << ::tts::FUNC(*found.first,*found.second)                          \
                              << " " << UNIT << " instead of " << N                               \
              );                                                                                  \
    return ::tts::logger{::tts::verbose_status};                                                  \
    }                                                                                             \
  }                                                                                               \
  else                                                                                            \
  {                                                                                               \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)       \
                            << ::tts::reset << " but sizes mismatch between: "                    \
                            << ::tts::red << std::size(LHS) << ::tts::reset                       \
                            << " and "  << ::tts::red << std::size(RHS) << ::tts::reset           \
            );                                                                                    \
    return ::tts::logger{::tts::verbose_status};                                                  \
  }                                                                                               \
}()
/**/

#define TTS_ALL_PRECISION(L,R,N,U,F, ...)     TTS_ALL_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F)
#define TTS_ALL_PRECISION_(L,R,N,U,F)         TTS_ALL_PRECISION_IMPL(L,R,N,U,F,TTS_FAIL)
#define TTS_ALL_PRECISION_REQUIRED(L,R,N,U,F) TTS_ALL_PRECISION_IMPL(L,R,N,U,F,TTS_FATAL)

#define TTS_ALL_ABSOLUTE_EQUAL(L,R,N,...) TTS_ALL_PRECISION(L,R,N,"unit",absolute_distance, __VA_ARGS__)
#define TTS_ALL_RELATIVE_EQUAL(L,R,N,...) TTS_ALL_PRECISION(L,R,N,"%"   ,relative_distance, __VA_ARGS__)
#define TTS_ALL_ULP_EQUAL(L,R,N,...)      TTS_ALL_PRECISION(L,R,N,"ULP" ,ulp_distance     , __VA_ARGS__)
#define TTS_ALL_IEEE_EQUAL(L,R ,...)      TTS_ALL_ULP_EQUAL(L,R,0., __VA_ARGS__)
#define TTS_ALL_EQUAL(L,R, ...)           TTS_ALL_ABSOLUTE_EQUAL(L,R,0, __VA_ARGS__)
