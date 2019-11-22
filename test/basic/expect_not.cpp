//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Check that counter-expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(a >  b);
  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(b <  a);
  TTS_EXPECT_NOT(b <= a);
}
