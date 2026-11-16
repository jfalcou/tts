//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "range.hpp"
#include <array>

int scale(float x, int k)
{
  return static_cast<int>(x) * k;
}

std::array<int, 4> ajar_scale(std::array<float, 4> const& x, std::array<int, 4> const& k)
{
  std::array<int, 4> that {scale(x[ 0 ], k[ 0 ]) + (k[ 0 ] % 2),
                           scale(x[ 1 ], k[ 1 ]) + (k[ 1 ] % 2),
                           scale(x[ 2 ], k[ 2 ]) + (k[ 2 ] % 2),
                           scale(x[ 3 ], k[ 3 ]) + (k[ 3 ] % 2)};
  return that;
}

TTS_CASE("Range check on a function of two arguments of different types")
{
  TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                      (tts::types<float, int>),
                      (tts::types<std::array<float, 4>, std::array<int, 4>>),
                      scale,
                      ajar_scale,
                      0.5);
};

// Passes only with k stuck at zero on the challenger side, which the tag refuses.
TTS_CASE(TTS_XFAIL("Range check on a function of two arguments reads each leaf"))
{
  TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                      (tts::types<float, int>),
                      (tts::types<std::array<float, 4>, std::array<int, 4>>),
                      scale,
                      ajar_scale,
                      0.4);
};

TTS_CASE("Range check with one producer shared by every argument")
{
  auto plus  = [](int a, int b) { return a + b; };
  auto aplus = [](std::array<int, 4> const& a, std::array<int, 4> const& b)
  {
    return std::array<int, 4> {a[ 0 ] + b[ 0 ], a[ 1 ] + b[ 1 ], a[ 2 ] + b[ 2 ], a[ 3 ] + b[ 3 ]};
  };

  TTS_ULP_RANGE_CHECK(tts::between(-50, 50),
                      (tts::types<int, int>),
                      (tts::types<std::array<int, 4>, std::array<int, 4>>),
                      plus,
                      aplus,
                      0.);
};

// A block of 4096 pairs is the product of 64 values on each side.
TTS_CASE("Range check on two arguments covers the product of the producers")
{
  static std::array<std::array<bool, 64>, 64> seen = {};

  auto                                        note = [](int a, int b)
  {
    seen[ static_cast<std::size_t>(a) ][ static_cast<std::size_t>(b) ] = true;
    return a - b;
  };
  auto anote = [](std::array<int, 4> const& a, std::array<int, 4> const& b)
  {
    return std::array<int, 4> {a[ 0 ] - b[ 0 ], a[ 1 ] - b[ 1 ], a[ 2 ] - b[ 2 ], a[ 3 ] - b[ 3 ]};
  };

  TTS_ULP_RANGE_CHECK(ramp,
                      (tts::types<int, int>),
                      (tts::types<std::array<int, 4>, std::array<int, 4>>),
                      note,
                      anote,
                      0.);

  int pairs = 0;
  for(auto const& row: seen)
    for(bool b: row)
      pairs += b ? 1 : 0;

  TTS_EQUAL(pairs, 4096);
};

// The first odd k sits at index 1 of the block: the 0.5 bucket marks the second lane of the first group.
TTS_CASE(
"Range check marks the element of each bucket in its group, under the call form of each function")
{
  ::tts::_::scoped_verbosity verbosity({.verbose = true, .quiet = false});

  tts::gathering_sink        gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                        (tts::types<float, int>),
                        (tts::types<std::array<float, 4>, std::array<int, 4>>),
                        scale,
                        ajar_scale,
                        0.5);
  }

  auto has = [ & ](char const* s)
  {
    return std::strstr(gs.content().data(), s) != nullptr; // NOSONAR - contains() is C++23
  };

  // Type names come from tts::typename_, whose spelling depends on the compiler
  TTS_EXPECT(has(tts::text("Comparing: scale(%s, %s) with ajar_scale(%s, %s)",
                           tts::as_text(tts::typename_<float>).data(),
                           tts::as_text(tts::typename_<int>).data(),
                           tts::as_text(tts::typename_<std::array<float, 4>>).data(),
                           tts::as_text(tts::typename_<std::array<int, 4>>).data())
                 .data()));
  TTS_EXPECT(has("Expression: ajar_scale similar to scale, at most 0.5 ULP apart"));
  TTS_EXPECT(has("Input:      { 0 *0* 0 0 }, { 0 *1* 2 3 }\n"));
  TTS_EXPECT(has("Found:      { 0 *1* 0 1 }\n"));
  TTS_EXPECT(has("instead of: { 0 *0* 0 0 }\n"));
};

// 4096 is no fifth power: the data set grows to 6^5 values, six per argument.
TTS_CASE("Range check on five arguments covers every combination of their values")
{
  static std::array<bool, 7776> seen = {};

  auto                          note = [](int a, int b, int c, int d, int e)
  {
    seen[ static_cast<std::size_t>((((a * 6 + b) * 6 + c) * 6 + d) * 6 + e) ] = true;
    return a;
  };

  TTS_ULP_RANGE_CHECK(ramp,
                      (tts::types<int, int, int, int, int>),
                      (tts::types<int, int, int, int, int>),
                      note,
                      note,
                      0.);

  int combinations = 0;
  for(bool b: seen)
    combinations += b ? 1 : 0;

  TTS_EQUAL(combinations, 7776);
};
