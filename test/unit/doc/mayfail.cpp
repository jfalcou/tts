//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE(TTS_MAYFAIL("Flaky under load on some CI runners"))
{
  TTS_EXPECT(true); // may occasionally fail elsewhere - either outcome is accepted here
};
//! [snippet]
