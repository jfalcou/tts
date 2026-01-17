//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>

TTS_CASE("Absolute distance")
{
  TTS_ABSOLUTE_EQUAL(-2.  , 2.f  , 4.);
  TTS_ABSOLUTE_EQUAL('A'  , 80LL , 15, REQUIRED);
};
//! [snippet]