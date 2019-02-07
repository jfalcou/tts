//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_BASIC_HPP_INCLUDED
#define TTS_TESTS_BASIC_HPP_INCLUDED

#include <tts/detail/decompose.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/tests/infos.hpp>

// Test pass if EXPR evaluates to true
#define TTS_EXPECT(EXPR)                                                                           \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_d = TTS_DECOMPOSE(EXPR);                                                          \
    if(tts_var_d)                                                                                  \
    {                                                                                              \
      TTS_PASS("Expecting: " << ::tts::detail::white_(TTS_STRING(EXPR)));                          \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting: " << ::tts::detail::red_(tts_var_d.lhs)                                 \
                             << " " << ::tts::detail::white_(tts_var_d.op) << " "                  \
                             << ::tts::detail::red_(tts_var_d.rhs));                               \
    }                                                                                              \
  } while(::tts::detail::is_false()) /**/

// Test pass if EXPR evaluates to false
#define TTS_EXPECT_NOT(EXPR)                                                                       \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_d = TTS_DECOMPOSE(EXPR);                                                          \
    if(tts_var_d)                                                                                  \
    {                                                                                              \
      TTS_FAIL("Not expecting: " << ::tts::detail::red_(tts_var_d.lhs)                             \
                             << " " << ::tts::detail::white_(tts_var_d.op) << " "                  \
                             << ::tts::detail::red_(tts_var_d.rhs));                               \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_PASS("Not expecting: " << ::tts::detail::white_(TTS_STRING(EXPR)));                      \
    }                                                                                              \
  } while(::tts::detail::is_false()) /**/

#endif
