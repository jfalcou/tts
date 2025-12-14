//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>

TTS_CASE( "Check that counter-expectation can be met" )
{
  constexpr int a = 42, b = 69;

  TTS_CONSTEXPR_EXPECT_NOT(a == b);
  TTS_CONSTEXPR_EXPECT_NOT(a >  b);
  TTS_CONSTEXPR_EXPECT_NOT(a >= b);
  TTS_CONSTEXPR_EXPECT_NOT(b <  a);
  TTS_CONSTEXPR_EXPECT_NOT(b <= a);
};
//! [snippet]
