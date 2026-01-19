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
#include <vector>

TTS_CASE("Check correctness of inequality tests")
{
  TTS_NOT_EQUAL(42, 69.f);

  std::vector<float> a {1, 2, 3}, b {7, 9};
  TTS_NOT_EQUAL(a, b);

  // This test would cause the program to stop with a failure
  // TTS_NOT_EQUAL('A', 65, REQUIRED);
};
//! [snippet]
