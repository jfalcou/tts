//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_BASIC_HPP_INCLUDED
#define TTS_TESTS_BASIC_HPP_INCLUDED

#include <tts/detail/decompose.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/tests/infos.hpp>

//==================================================================================================
/**
  * @brief Checks if a given expression evaluates to `true`.
  *
  * **Required header:** `#include <tts/tests/basic.hpp>`
  *
  *  **Example:**
  *
  *  @snippet basic/expect.cpp expect
  *
  *  @param Expr Expression to test
**/
//==================================================================================================
#define TTS_EXPECT(Expr)                                                                           \
  do                                                                                               \
  {                                                                                                \
    ::tts::detail::result tts_var_d = TTS_DECOMPOSE(Expr);                                         \
    if(tts_var_d) { TTS_PASS("Expecting: " << ::tts::detail::white_(TTS_STRING(Expr))); }          \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting: " << ::tts::detail::red_(tts_var_d.lhs) << " "                          \
                             << ::tts::detail::white_(tts_var_d.op) << " "                         \
                             << ::tts::detail::red_(tts_var_d.rhs));                               \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
/**
  * @brief Checks if a given expression evaluates to `false`.
  *
  * **Required header:** `#include <tts/tests/basic.hpp>`
  *
  *  **Example:**
  *
  *  @snippet basic/expect_not.cpp expect-not
  *
  *  @param Expr Expression to test
**/
//==================================================================================================
#define TTS_EXPECT_NOT(Expr)                                                                       \
  do                                                                                               \
  {                                                                                                \
    ::tts::detail::result tts_var_d = TTS_DECOMPOSE(Expr);                                         \
    if(tts_var_d)                                                                                  \
    {                                                                                              \
      TTS_FAIL("Not expecting: " << ::tts::detail::red_(tts_var_d.lhs) << " "                      \
                                 << ::tts::detail::white_(tts_var_d.op) << " "                     \
                                 << ::tts::detail::red_(tts_var_d.rhs));                           \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_PASS("Not expecting: " << ::tts::detail::white_(TTS_STRING(Expr)));                      \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#endif

