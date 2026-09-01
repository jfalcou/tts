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
  TTS_RELATIVE_EQUAL(static_cast<long long>('A'), 80LL, 0.1875);
  TTS_RELATIVE_EQUAL(1., 2., 1.);
  TTS_RELATIVE_EQUAL(1, 10, 9., REQUIRED);
};
//! [snippet]
