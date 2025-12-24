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

namespace sample
{
  struct ratio { int n, d; };

  double relative_distance(ratio a, ratio b)
  {
    auto ra = static_cast<float>(a.n) / a.d;
    auto rb = static_cast<float>(b.n) / b.d;

    return tts::relative_check(ra, rb);
  }
};

TTS_CASE( "Compare values with custom relative distance computation" )
{
  sample::ratio a{1,77};
  sample::ratio b{3,85};

  TTS_RELATIVE_EQUAL(a, b, 2.25);
};
//! [snippet]
