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

// A user-defined type for which we will provide custom limits
struct data
{
  int   i;
  float f;
};

// Specialize the limits_set for our user-defined type
template<> struct tts::limits_set<data>
{
  data valmax {std::numeric_limits<int>::max(), std::numeric_limits<float>::max()};
  data valmin {std::numeric_limits<int>::lowest(), std::numeric_limits<float>::lowest()};
  data zero {0, 0.0f};
  data one {1, 1.0f};
};

TTS_CASE("limits generator for float")
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

TTS_CASE("limits generator for data")
{
  // Create a limits generator for data
  auto l = tts::limits(tts::type<data> {});

  // Basic sanity checks
  TTS_EQUAL(l.valmax.i, std::numeric_limits<int>::max());
  TTS_EQUAL(l.valmax.f, std::numeric_limits<float>::max());
  TTS_EQUAL(l.valmin.i, std::numeric_limits<int>::lowest());
  TTS_EQUAL(l.valmin.f, std::numeric_limits<float>::lowest());
  TTS_EQUAL(l.zero.i, 0);
  TTS_EQUAL(l.zero.f, 0.0f);
  TTS_EQUAL(l.one.i, 1);
  TTS_EQUAL(l.one.f, 1.0f);
};
//! [snippet]
