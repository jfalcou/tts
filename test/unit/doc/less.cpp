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

TTS_CASE("Check correctness of less-than comparison tests")
{
  TTS_LESS(42., 69);

  std::vector<float> a {1, 2, 3}, b {12, 3};
  TTS_LESS(a, b);

  // This test would cause the program to stop with a failure
  // TTS_LESS(69, 42, REQUIRED);
};
//! [snippet]
