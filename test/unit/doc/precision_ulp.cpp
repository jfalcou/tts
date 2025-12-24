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

  float ulp_distance(ratio a, ratio b)
  {
    auto ra = static_cast<float>(a.n) / a.d;
    auto rb = static_cast<float>(b.n) / b.d;

    return tts::ulp_check(ra, rb);
  }
};

TTS_CASE( "Compare values with custom ULP distance computation" )
{
  sample::ratio a{8388608,8388608};
  sample::ratio b{8388618,8388608};

  TTS_ULP_EQUAL(a, b,10);
};
//! [snippet]
