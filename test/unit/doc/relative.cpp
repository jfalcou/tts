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

TTS_CASE("Relative distance")
{
  TTS_RELATIVE_EQUAL(42.f, 42.f, 0);
  TTS_RELATIVE_EQUAL(static_cast<long long>('A'), 80LL, 18.75);
  TTS_RELATIVE_EQUAL(1., 2., 100.);
  TTS_RELATIVE_EQUAL(1, 10, 900., REQUIRED);
};
//! [snippet]
