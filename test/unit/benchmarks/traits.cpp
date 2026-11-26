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
#include <concepts>

// The double function runs on the float buffer, converted to double
struct widening : tts::bench::default_traits
{
  template<typename Row, typename Value>
  static decltype(auto) input(tts::buffer<Value> const& values)
  {
    if constexpr(std::same_as<Row, double>) return tts::bench::convert<double>(values);
    else return default_traits::input<Row>(values);
  }

  static tts::text environment()
  {
    return tts::text("Input data generated in float");
  }
};

TTS_CASE("Benchmark std::sqrt in float and in double")
{
  auto x = tts::bench::random<float>(1.f, 100.f);

  // Both rows share one table under the widening traits
  tts::bench::basic_benchmark<widening> b("sqrt by precision");
  TTS_RUN_BENCHMARK(b, float, "float", [](float v) { return std::sqrt(v); }, x);
  TTS_RUN_BENCHMARK(b, double, "double", [](double v) { return std::sqrt(v); }, x);

  TTS_PASS("Benchmarked");
};
//! [snippet]
