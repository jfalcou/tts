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
  // Two buffers drawn once: the i-th call of every row takes x[i] and y[i]
  auto                  x = tts::bench::random<double>(-100., 100.);
  auto                  y = tts::bench::random<double>(-100., 100.);

  tts::bench::benchmark b("hypot");
  TTS_RUN_BENCHMARK(
  b, double, "std::hypot", [](double u, double v) { return std::hypot(u, v); }, x, y);
  TTS_RUN_BENCHMARK(
  b, double, "naive", [](double u, double v) { return std::sqrt(u * u + v * v); }, x, y);

  TTS_PASS("Timed");
};
//! [snippet]
