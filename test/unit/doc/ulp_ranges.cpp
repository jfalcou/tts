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

float ok_x(float x)
{
  return x;
}
float bad_x(float x)
{
  return x + x * 1e-7f;
}

TTS_CASE("Test range check")
{
  TTS_ULP_RANGE_CHECK(
  [](auto, auto i, auto c) { return (100.f * i) / c; }, float, float, ok_x, bad_x, 2.);
};
//! [snippet]
