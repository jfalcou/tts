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

namespace sample
{
  struct ratio
  {
    int n, d;
  };

  double absolute_distance(ratio a, ratio b)
  {
    auto ra = static_cast<float>(a.n) / a.d;
    auto rb = static_cast<float>(b.n) / b.d;

    return tts::absolute_check(ra, rb);
  }
};

TTS_CASE("Compare values with custom absolute distance computation")
{
  sample::ratio a {115, 77};
  sample::ratio b {397, 85};

  TTS_ABSOLUTE_EQUAL(a, b, 4);
};
//! [snippet]
