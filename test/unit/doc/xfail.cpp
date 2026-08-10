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

TTS_CASE(TTS_XFAIL("Known, not-yet-fixed rounding bug"))
{
  TTS_EQUAL(1 + 1, 3); // stands in for the real, currently broken behavior being tracked
};
//! [snippet]
