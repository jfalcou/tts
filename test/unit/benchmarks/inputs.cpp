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
  double lo;
  double hi;
};

TTS_CASE("Generate a buffer of intervals and a buffer of flags")
{
  // Each interval is filled from two random values, a lower bound and a width
  tts::buffer<interval> values(tts::bench::size);
  for(auto& v: values)
  {
    v.lo = tts::bench::roll(-1., 1.);
    v.hi = v.lo + tts::bench::roll(0., 0.5);
  }

  auto flags = tts::bench::random<bool>(false, true, 64);

  TTS_LESS_EQUAL(values[ 0 ].lo, values[ 0 ].hi);
  TTS_EQUAL(flags.size(), std::size_t {64});
};
//! [snippet]
