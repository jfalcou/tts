//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check that expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT(a != b);
  TTS_EXPECT(a <  b);
  TTS_EXPECT(a <= b);
  TTS_EXPECT(b >  a);
  TTS_EXPECT(b >= a);
}
