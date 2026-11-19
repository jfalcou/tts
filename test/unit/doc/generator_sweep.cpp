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

TTS_CASE("Check two functions over swept values")
{
  auto halve_ref = [](std::int16_t x) { return static_cast<std::int16_t>(std::floor(x / 2.)); };
  auto halve_new = [](std::int16_t x) { return static_cast<std::int16_t>(x >> 1); };

  // The 65536 values of std::int16_t fit the scope of 2^24: each one is checked
  TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, halve_ref, halve_new, 0.);

  auto half   = [](float x) { return x / 2; };
  auto scaled = [](float x) { return x * 0.5f; };

  // The floats from -1 to 1 do not: 2^24 of them are checked, spread over their binades
  TTS_ULP_RANGE_CHECK(tts::sweep(-1.f, 1.f), float, float, half, scaled, 0.);

  // A scope of its own: 4096 calls for this check
  TTS_ULP_RANGE_CHECK(tts::sweep(-1.f, 1.f, 4096), float, float, half, scaled, 0.);
};
//! [snippet]
