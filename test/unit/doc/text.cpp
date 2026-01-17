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

TTS_CASE("Using tts::text")
{
  tts::text hello("Hello ");
  tts::text world("World !");
  tts::text qty(" with values %d %f", 42, 3.5f);
  tts::text full = hello + world + qty;

  TTS_EQUAL(full, "Hello World ! with values 42 3.500000");
};
//! [snippet]