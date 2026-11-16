//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "range.hpp"
#include <array>
#include <limits>

int ok_x(float x)
{
  return static_cast<int>(x);
}

int ajar_x(float x)
{
  return static_cast<int>((int(x) % 2 == 1) ? x : x + x);
}

std::array<int, 4> ajar_ax(std::array<float, 4> const& x)
{
  std::array<int, 4> that {ajar_x(x[ 0 ]), ajar_x(x[ 1 ]), ajar_x(x[ 2 ]), ajar_x(x[ 3 ])};
  return that;
}

TTS_CASE("Test stateless range check")
{
  TTS_ULP_RANGE_CHECK(tts::between(0.f, 100.f), float, (std::array<float, 4>), ok_x, ajar_ax, 64);
};

TTS_CASE("Test stateful range check")
{
  TTS_ULP_RANGE_CHECK(
  tts::realistic_generator<float>(-10, 10), float, (std::array<float, 4>), ok_x, ajar_ax, 128);
};

TTS_CASE("Range check next to a local named like a macro local")
{
  auto generator = tts::between(0.f, 100.f);
  TTS_ULP_RANGE_CHECK(generator, float, (std::array<float, 4>), ok_x, ajar_ax, 64);
};

TTS_CASE("Range check that passes prints nothing without --verbose")
{
  ::tts::_::scoped_verbosity verbosity({.verbose = false, .quiet = false});

  tts::gathering_sink        gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK(tts::between(0.f, 100.f), float, (std::array<float, 4>), ok_x, ajar_ax, 64);
  }

  TTS_EXPECT(gs.content().is_empty());
};

TTS_CASE("Range check hands its generators the signed index of TTS_CASE_WITH")
{
  bool signed_index = false;
  auto probe        = [ & ]<typename I>(auto, I i, auto)
  {
    signed_index = std::is_signed_v<I>;
    return static_cast<int>(i);
  };

  auto same = [](int x) { return x; };
  TTS_ULP_RANGE_CHECK(probe, int, int, same, same, 0.);
  TTS_EXPECT(signed_index);
};

// (1. + 1.) * 2. is a bound of 4, and 1. + 1. * 2. one of 3.
TTS_CASE("Range check takes a bound that opens with a parenthesis")
{
  auto same = [](int x) { return x; };
  auto off8 = [](int x) { return x + 8; };
  TTS_ULP_RANGE_CHECK(ramp, int, int, same, off8, (1. + 1.) * 2.);
};

TTS_CASE(TTS_XFAIL("Range check fails on a bound that is not a number"))
{
  auto same = [](int x) { return x; };
  TTS_ULP_RANGE_CHECK(ramp, int, int, same, same, std::numeric_limits<double>::quiet_NaN());
};
