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

TTS_CASE("Check correctness of constexpr less-or-equal-than tests")
{
  constexpr int   a = 45;
  constexpr float b = 4.5f;

  TTS_CONSTEXPR_GREATER_EQUAL(a, b);
  TTS_CONSTEXPR_GREATER_EQUAL(a, a);
};
//! [snippet]
