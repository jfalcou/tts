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
#include <cmath>

TTS_CASE("Check the time per element of std::hypot and of the naive formula")
{
  auto x = tts::bench::random<double>(-100., 100.);
  auto y = tts::bench::random<double>(-100., 100.);

  // Functions to compare, each called on one element of x and one element of y
  auto hypot = [](double u, double v) { return std::hypot(u, v); };
  auto naive = [](double u, double v) { return std::sqrt(u * u + v * v); };

  // One table of results, whose first row is the baseline
  tts::bench::benchmark b("hypot");

  // Each TTS_RUN_BENCHMARK returns the measurement of its row
  auto reference = TTS_RUN_BENCHMARK(b, double, "std::hypot", hypot, x, y);
  auto candidate = TTS_RUN_BENCHMARK(b, double, "naive", naive, x, y);

  // Both functions take a few nanoseconds per element, far under a bound of one microsecond
  TTS_LESS(reference.time, 1.);
  TTS_LESS(candidate.time, 1.);
};

TTS_CASE("Check that the median epoch of std::hypot lies between the fastest and the slowest")
{
  auto x = tts::bench::random<double>(-100., 100.);
  auto y = tts::bench::random<double>(-100., 100.);

  // One table of results, with a single row
  tts::bench::benchmark b("hypot epochs");

  // A structured binding takes the four fields in their order of declaration
  auto [ time, fastest, slowest, error ] = TTS_RUN_BENCHMARK(
  b, double, "std::hypot", [](double u, double v) { return std::hypot(u, v); }, x, y);

  TTS_LESS_EQUAL(fastest, time);
  TTS_LESS_EQUAL(time, slowest);
  TTS_GREATER_EQUAL(error, 0.);
};
//! [snippet]
