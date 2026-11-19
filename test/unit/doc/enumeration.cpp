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

TTS_CASE("Ranks of the values of a float")
{
  using ranks = tts::enumeration<float>;

  // -0 and +0 take two neighboring ranks
  TTS_EQUAL(ranks::index(0.f), ranks::index(-0.f) + 1);
  TTS_LESS(ranks::index(-1.f), ranks::index(1.f));
  TTS_EQUAL(ranks::value(ranks::index(1.5f)), 1.5f);
};
//! [snippet]
