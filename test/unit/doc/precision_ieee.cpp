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

  double ieee_equal(ratio a, ratio b)
  {
    auto ra = static_cast<float>(a.n) / a.d;
    auto rb = static_cast<float>(b.n) / b.d;
    printf("%f vs %f\n", ra, rb);
    return tts::ieee_check(ra, rb);
  }
};

TTS_CASE("Compare values with custom IEEE distance computation")
{
  sample::ratio a {189, 265};
  sample::ratio b {0, 0};

  TTS_IEEE_EQUAL(a, a);
  TTS_IEEE_EQUAL(b, b);
};
//! [snippet]
