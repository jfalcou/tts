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
#include <vector>

TTS_CASE( "Check correctness of equality tests" )
{
  TTS_EQUAL(45, 45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_EQUAL(a,b);

  // This test would cause the program to stop with a failure.
  // TTS_EQUAL(12, 21, REQUIRED);
};
//! [snippet]