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
  constexpr int a = 42, b = 69;

  TTS_CONSTEXPR_EXPECT(a != b);
  TTS_CONSTEXPR_EXPECT(a < b);
  TTS_CONSTEXPR_EXPECT(a <= b);
  TTS_CONSTEXPR_EXPECT(b > a);
  TTS_CONSTEXPR_EXPECT(b >= a);
};
//! [snippet]
