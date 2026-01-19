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

void bar() {}

TTS_CASE("Check that nothrow function are detected as such") { TTS_NO_THROW(bar()); };
//! [snippet]
