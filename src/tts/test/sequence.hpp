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
#include <type_traits>

namespace tts::detail
{
  template<typename T, typename U> struct failure
  {
    std::size_t index;
    T original;
    U other;
  };
}

#define TTS_ALL_IMPL(SEQ1,SEQ2,OP,N,UNIT,FAILURE)                                                   \
[](auto const& a, auto const& b)                                                                    \
{                                                                                                   \
  if( std::size(b) != std::size(a) )                                                                \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but sizes does not match: "                                        \
                            << "size(" TTS_STRING(SEQ1) ") = " << std::size(a)                      \
                            << " while size(" TTS_STRING(SEQ2) ") = " << std::size(b)               \
            );                                                                                      \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
                                                                                                    \
  auto ba = std::begin(a);                                                                          \
  auto bb = std::begin(b);                                                                          \
  auto ea = std::end(a);                                                                            \
                                                                                                    \
  std::vector < ::tts::detail::failure< std::remove_cvref_t<decltype(*ba)>                          \
                                      , std::remove_cvref_t<decltype(*bb)>                          \
                                      >                                                             \
              > failures;                                                                           \
  std::size_t i = 0;                                                                                \
                                                                                                    \
  while(ba != ea)                                                                                   \
  {                                                                                                 \
    if( OP(*ba,*bb) > N )  failures.push_back({i++,*ba,*bb});                                       \
    ba++;                                                                                           \
    bb++;                                                                                           \
  }                                                                                                 \
                                                                                                    \
  if( !failures.empty( ) )                                                                          \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but values differ from more than " << N << " "<< UNIT              \
            );                                                                                      \
                                                                                                    \
    for(auto f : failures)                                                                          \
      std::cout << "    @[" << f.index << "] : " << f.original << " and " << f.other                \
                << " differ by " << OP(f.original,f.other) << " " << UNIT << "\n";                  \
                                                                                                    \
    std::cout << "\n";                                                                              \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
                                                                                                    \
  ::tts::global_runtime.pass();                                                                     \
  return ::tts::logger{false};                                                                      \
}(SEQ1, SEQ2)                                                                                       \
/**/

#define TTS_ALL(L,R,F,N,U, ...)     TTS_ALL_ ## __VA_ARGS__ (L,R,F,N,U)
#define TTS_ALL_(L,R,F,N,U)         TTS_ALL_IMPL(L,R,F,N,U,TTS_FAIL)
#define TTS_ALL_REQUIRED(L,R,F,N,U) TTS_ALL_IMPL(L,R,F,N,U,TTS_FATAL)

#define TTS_ALL_ABSOLUTE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::absolute_distance,N,"unit" __VA_ARGS__ )
#define TTS_ALL_RELATIVE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::relative_distance,N,"%"    __VA_ARGS__ )
#define TTS_ALL_ULP_EQUAL(L,R,N,...)      TTS_ALL(L,R, ::tts::ulp_distance     ,N,"ULP"  __VA_ARGS__ )
#define TTS_ALL_IEEE_EQUAL(S1,S2,...)     TTS_ALL_ULP_EQUAL(S1,S2,0, __VA_ARGS__)
