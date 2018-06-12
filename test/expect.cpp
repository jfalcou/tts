//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>

//! [expect]
TTS_CASE( "Check that expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT(a != b);
  TTS_EXPECT(a <  b);
  TTS_EXPECT(a <= b);
  TTS_EXPECT(b >  a);
  TTS_EXPECT(b >= a);
}
//! [expect]

//! [expect_not]
TTS_CASE( "Check that counter-expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(a >  b);
  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(b <  a);
  TTS_EXPECT_NOT(b <= a);
}
//! [expect_not]
