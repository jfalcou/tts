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

TTS_CASE( "Check that expectation can't be met" )
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(b < a);

  // This test would cause the program to stop and not run the remaining tests
  // TTS_EXPECT_NOT(a != b, REQUIRED);

  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(a >  b);
};
//! [snippet]