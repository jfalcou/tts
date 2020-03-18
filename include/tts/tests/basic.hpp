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
    if(tts_var_d)                                                                                  \
    {                                                                                              \
      TTS_PASS("Expecting: "  << ::tts::detail::white                                              \
                              << TTS_STRING(Expr) << ::tts::detail::reset                          \
              );                                                                                   \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL( "Expecting: " << ::tts::detail::white << TTS_STRING(Expr)                          \
                              << ::tts::detail::reset << " but "                                   \
                              << ::tts::detail::red                                                \
                              << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs      \
                              << ::tts::detail::reset << " occured instead.");                     \
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
      TTS_FAIL( "Not Expecting: " << ::tts::detail::white << TTS_STRING(Expr)                      \
                                  << ::tts::detail::reset << " but "                               \
                                  << ::tts::detail::red                                            \
                                  << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs  \
                                  << ::tts::detail::reset << " occured anyway.");                  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_PASS("Not Expecting: "  << ::tts::detail::white                                          \
                                  << TTS_STRING(Expr) << ::tts::detail::reset                      \
              );                                                                                   \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#endif

