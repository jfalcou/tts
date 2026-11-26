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

TTS_CASE("Benchmark std::hypot against the naive formula")
{
  // Input data: two buffers of 1024 random values
  auto x = tts::bench::random<double>(-100., 100.);
  auto y = tts::bench::random<double>(-100., 100.);

  // Functions to compare, each called on one element of x and one element of y
  auto hypot = [](double u, double v) { return std::hypot(u, v); };
  auto naive = [](double u, double v) { return std::sqrt(u * u + v * v); };

  // One table of results, whose first row is the baseline
  tts::bench::benchmark b("hypot");
  TTS_RUN_BENCHMARK(b, double, "std::hypot", hypot, x, y);
  TTS_RUN_BENCHMARK(b, double, "naive", naive, x, y);

  TTS_PASS("Benchmarked");
};
//! [snippet]
