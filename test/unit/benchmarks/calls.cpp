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

double halve(double v)
{
  return v / 2;
}

TTS_CASE("Benchmark a lambda, a function name, and an overloaded function")
{
  auto x = tts::bench::random<double>(-10., 10.);

  // The lambda and halve compute the same division
  tts::bench::benchmark b("calls");
  TTS_RUN_BENCHMARK(b, double, "lambda", [](double v) { return v / 2; }, x);
  TTS_RUN_BENCHMARK(b, double, "function name", halve, x);
  // std::exp is overloaded, so call it from a lambda
  TTS_RUN_BENCHMARK(b, double, "std::exp", [](double v) { return std::exp(v); }, x);

  TTS_PASS("Benchmarked");
};
//! [snippet]
