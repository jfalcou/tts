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

#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/decompose.hpp>
#include <tts/tests/infos.hpp>

// Test pass if EXPR evaluates to true
#define TTS_EXPECT( EXPR )                                    \
do                                                            \
{                                                             \
  if( ::tts::detail::result tts_var_r = TTS_DECOMPOSE(EXPR) ) \
    TTS_PASS( "Expecting: " << TTS_STRING(EXPR));             \
  else                                                        \
  {                                                           \
    TTS_FAIL( "Expecting: " << TTS_STRING(EXPR));             \
  }                                                           \
} while( ::tts::detail::is_false() )                          \
/**/

// Test pass if EXPR evaluates to false
#define TTS_EXPECT_NOT( EXPR )                                \
do                                                            \
{                                                             \
  if( ::tts::detail::result tts_var_r = TTS_DECOMPOSE(EXPR) ) \
  {                                                           \
    TTS_FAIL( "Not expecting: " << TTS_STRING(EXPR));         \
  }                                                           \
  else                                                        \
    TTS_PASS( "Not expecting: " << TTS_STRING(EXPR));         \
} while( ::tts::detail::is_false() )                          \
/**/

#endif
