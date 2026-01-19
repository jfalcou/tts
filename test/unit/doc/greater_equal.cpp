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

TTS_CASE("Check correctness of greater-equal comparison tests")
{
  TTS_GREATER_EQUAL(69LL, 42.f);
  TTS_GREATER_EQUAL(69., 69);

  std::vector<float> a {12, 3}, b {7, 9, 11};
  TTS_GREATER_EQUAL(a, b);
  TTS_GREATER_EQUAL(a, a);

  // This test would cause the program to stop with a failure
  // TTS_GREATER_EQUAL(1., 10., REQUIRED);
};
//! [snippet]
