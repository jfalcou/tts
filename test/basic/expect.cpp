//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check that expectation can be met" )
{
  int a = 43, b = 69;

  TTS_EXPECT(a == a);
  TTS_EXPECT(a != b);
  TTS_EXPECT(a <  b);
  TTS_EXPECT(a <= b);
  TTS_EXPECT(b >  a);
  TTS_EXPECT(b >= a);
  TTS_EXPECT(a && b);
  TTS_EXPECT(a || b);

  TTS_EXPECT(a == a, REQUIRED);
  TTS_EXPECT(a != b, REQUIRED);
  TTS_EXPECT(a <  b, REQUIRED);
  TTS_EXPECT(a <= b, REQUIRED);
  TTS_EXPECT(b >  a, REQUIRED);
  TTS_EXPECT(b >= a, REQUIRED);
  TTS_EXPECT(a && b, REQUIRED);
  TTS_EXPECT(a || b, REQUIRED);
};

constexpr bool eq (int a, int b) { return a == b; }
constexpr bool neq(int a, int b) { return a != b; }
constexpr bool lt (int a, int b) { return a < b;  }
constexpr bool gt (int a, int b) { return a > b;  }
constexpr bool lte(int a, int b) { return a <= b; }
constexpr bool gte(int a, int b) { return a >= b; }

TTS_CASE( "Check that constexpr expectation can be met" )
{
  TTS_CONSTEXPR_EXPECT(true);
  TTS_CONSTEXPR_EXPECT(eq (42, 42));
  TTS_CONSTEXPR_EXPECT(neq(69, 42));
  TTS_CONSTEXPR_EXPECT(lt (42, 69));
  TTS_CONSTEXPR_EXPECT(gt (69, 42));
  TTS_CONSTEXPR_EXPECT(lte(42, 69));
  TTS_CONSTEXPR_EXPECT(gte(69, 42));
};
