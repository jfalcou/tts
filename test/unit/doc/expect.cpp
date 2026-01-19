//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

TTS_CASE("Check that expectation can be met")
{
  int a = 42, b = 69;

  TTS_EXPECT(a < b);
  TTS_EXPECT(b > a);
  TTS_EXPECT(a != b);

  // This test would cause the program to stop and not run the remaining tests
  // TTS_EXPECT(a == b, REQUIRED);

  TTS_EXPECT(a <= b);
  TTS_EXPECT(b >= a);
};
//! [snippet]
