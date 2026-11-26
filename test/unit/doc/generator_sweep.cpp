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
#include <cmath>
#include <cstdint>

TTS_CASE("Check two functions over every value of a type or a range")
{
  auto halve_ref = [](std::int16_t x) { return static_cast<std::int16_t>(std::floor(x / 2.)); };
  auto halve_new = [](std::int16_t x) { return static_cast<std::int16_t>(x >> 1); };

  // Each of the 65536 values of std::int16_t
  TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, halve_ref, halve_new, 0.);

  auto half   = [](float x) { return x / 2; };
  auto scaled = [](float x) { return x * 0.5f; };

  // Each float from 1 to 2, 8388609 values
  TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 2.f), float, float, half, scaled, 0.);
};
//! [snippet]
