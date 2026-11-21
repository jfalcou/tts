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

TTS_CASE("Check two functions over values spread over a range")
{
  auto half   = [](float x) { return x / 2; };
  auto scaled = [](float x) { return x * 0.5f; };

  // 4096 floats from -1 to 1: the bounds and the same number of values in every binade
  TTS_ULP_RANGE_CHECK(tts::sample(-1.f, 1.f, 4096), float, float, half, scaled, 0.);

  auto twice = [](double x) { return x * 2; };
  auto added = [](double x) { return x + x; };

  // 2^20 doubles spread over the whole type, whose values are too many to run each
  TTS_ULP_RANGE_CHECK(tts::sample(1 << 20), double, double, twice, added, 0.);
};
//! [snippet]
