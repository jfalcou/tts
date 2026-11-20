//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/benchmarks.hpp>

struct interval
{
  double lo, hi;
};

TTS_CASE("Draw a buffer of intervals, and the same values from the same seed")
{
  // An interval takes two values of the generator of the benchmarks, its width on [0, 0.5]
  tts::buffer<interval> values(tts::bench::size);
  for(auto& v: values)
  {
    double const lo = tts::bench::roll(-1., 1.);
    v               = {lo, lo + tts::bench::roll(0., 0.5)};
  }

  tts::bench::prng_state = 7;
  auto first             = tts::bench::random<int>(0, 9);
  tts::bench::prng_state = 7;
  auto again             = tts::bench::random<int>(0, 9);

  TTS_LESS_EQUAL(values[ 0 ].lo, values[ 0 ].hi);
  TTS_EQUAL(first[ 0 ], again[ 0 ]);
};
//! [snippet]
