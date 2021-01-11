//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check that counter-expectation can be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(a >  b);
  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(b <  a);
  TTS_EXPECT_NOT(b <= a);

  TTS_EXPECT_NOT(b & a);
}

constexpr bool eq (int a, int b) { return a == b; }
constexpr bool neq(int a, int b) { return a != b; }
constexpr bool lt (int a, int b) { return a < b;  }
constexpr bool gt (int a, int b) { return a > b;  }
constexpr bool lte(int a, int b) { return a <= b; }
constexpr bool gte(int a, int b) { return a >= b; }

TTS_CASE( "Check that constexpr counter-expectation can be met" )
{
  TTS_CONSTEXPR_EXPECT_NOT(eq (69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(neq(42, 42));
  TTS_CONSTEXPR_EXPECT_NOT(lt (69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(gt (42, 69));
  TTS_CONSTEXPR_EXPECT_NOT(lte(69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(gte(42, 69));
}
