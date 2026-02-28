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

TTS_CASE("limits generator example")
{
  // Create a limits generator for float
  auto l = tts::limits(tts::type<float> {});

  // Basic sanity checks
  TTS_EXPECT(l.nan != l.nan);
  TTS_EQUAL(l.inf, std::numeric_limits<float>::infinity());
  TTS_EQUAL(l.valmax, std::numeric_limits<float>::max());
  TTS_EQUAL(l.valmin, std::numeric_limits<float>::lowest());
  TTS_EQUAL(l.maxflint, static_cast<float>(1u << std::numeric_limits<float>::digits));
};
//! [snippet]
