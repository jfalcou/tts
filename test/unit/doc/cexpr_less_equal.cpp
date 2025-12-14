//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of constexpr less-or-equal-than tests" )
{
  constexpr float a = 4.5f;
  constexpr int   b = 45;

  TTS_CONSTEXPR_LESS_EQUAL(a, b);
  TTS_CONSTEXPR_LESS_EQUAL(a, a);
};
//! [snippet]