//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

//==================================================================================================
// Warning Suppressors
//==================================================================================================
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

//==================================================================================================
// Test engine
//==================================================================================================
#include <tts/engine/main.hpp>
#include <tts/test/case.hpp>
#include <tts/test/info.hpp>

//==================================================================================================
// Test macros
//==================================================================================================
#include <tts/test/basic.hpp>
#include <tts/test/exceptions.hpp>
#include <tts/test/precision.hpp>
#include <tts/test/relation.hpp>
#include <tts/test/types.hpp>

#if 0
//==================================================================================================
// Misc. Helpers
//==================================================================================================
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

//==================================================================================================
// Test macros - Sequence tests
//==================================================================================================
#define TTS_SEQUENCE_EQUAL(L,R,N,UNIT,FUNC)                                                         \
  do                                                                                                \
  {                                                                                                 \
    if( std::size(L) == std::size(R) )                                                              \
    {                                                                                               \
      auto found = tts::detail::mismatch( std::begin(L), std::end(L), std::begin(R)                 \
                                        , [](auto l, auto r) { return FUNC(l,r) <= N; }             \
                                        );                                                          \
      auto distance = std::end(L)-found.first;                                                      \
      if( distance == 0)                                                                            \
      {                                                                                             \
        TTS_PASS( "Expecting: " << ::tts::green()                                                   \
                << TTS_STRING(L) << " == " << TTS_STRING(R) << ::tts::reset()                       \
                << " within " << ::tts::green() << N << ::tts::reset() << " " << UNIT               \
                );                                                                                  \
      }                                                                                             \
      else                                                                                          \
      {                                                                                             \
        TTS_FAIL(  "Expecting: " << ::tts::green() << TTS_STRING(L) << " == " << TTS_STRING(R)      \
                << ::tts::reset()                                                                   \
                << " but value at index " << ::tts::yellow() << distance    << ::tts::reset()       \
                << " is " << ::tts::red() << ::tts::as_string(*found.first) << ::tts::reset()       \
                << " instead of "                                                                   \
                << ::tts::red() << ::tts::as_string(*found.second)<< ::tts::reset()                 \
                << " within " << ::tts::red() << FUNC(*found.first,*found.second)                   \
                << " " << UNIT << " instead of " << N                                               \
                );                                                                                  \
      }                                                                                             \
    }                                                                                               \
    else                                                                                            \
    {                                                                                               \
      TTS_FAIL(  "Expecting: " << TTS_STRING(L) << " == " << TTS_STRING(R)                          \
              << " but sizes mismatch between: " << ::tts::red() << std::size(L) << ::tts::reset()  \
              << " and "  << ::tts::red() << std::size(R) << ::tts::reset()                         \
              );                                                                                    \
    }                                                                                               \
  } while(::tts::detail::done())
/**/

#define TTS_ALL_RELATIVE_EQUAL(L, R, N) TTS_SEQUENCE_EQUAL(L,R,N,"%"   , ::tts::relative_distance )
#define TTS_ALL_ULP_EQUAL(L, R, N)      TTS_SEQUENCE_EQUAL(L,R,N,"ULP" , ::tts::ulp_distance      )
#define TTS_ALL_ABSOLUTE_EQUAL(L, R, N) TTS_SEQUENCE_EQUAL(L,R,N,"unit", ::tts::absolute_distance )
#define TTS_ALL_EQUAL(L,R)              TTS_ALL_ABSOLUTE_EQUAL(L,R,0)
#define TTS_ALL_IEEE_EQUAL(L,R)         TTS_ALL_ULP_EQUAL(L, R, 0.)

#endif
