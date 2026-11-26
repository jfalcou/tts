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

TTS_CASE("Benchmark std::cbrt over 21 epochs")
{
  auto x = tts::bench::random<double>(0., 100.);

  // 21 epochs after 10 warmup passes for every function of this benchmark
  tts::bench::benchmark b("cbrt", 21, 10);
  TTS_RUN_BENCHMARK(b, double, "std::cbrt", [](double v) { return std::cbrt(v); }, x);

  TTS_PASS("Benchmarked");
};
//! [snippet]
