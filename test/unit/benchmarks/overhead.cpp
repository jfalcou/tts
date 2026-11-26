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

TTS_CASE("Benchmark std::hypot and the naive formula with an overhead row")
{
  auto x = tts::bench::random<double>(-100., 100.);
  auto y = tts::bench::random<double>(-100., 100.);

  // The third function computes next to nothing, and still reads both buffers
  auto hypot = [](double u, double v) { return std::hypot(u, v); };
  auto naive = [](double u, double v) { return std::sqrt(u * u + v * v); };
  auto adder = [](double u, double v) { return u + v; };

  // The overhead row comes last, so std::hypot stays the baseline
  tts::bench::benchmark b("hypot");
  TTS_RUN_BENCHMARK(b, double, "std::hypot", hypot, x, y);
  TTS_RUN_BENCHMARK(b, double, "naive", naive, x, y);
  TTS_RUN_BENCHMARK(b, double, "overhead", adder, x, y);

  TTS_PASS("Benchmarked");
};
//! [snippet]
