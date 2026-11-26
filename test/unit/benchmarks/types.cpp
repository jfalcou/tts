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

// One buffer generated in float, converted for every type
tts::buffer<float> const& inputs()
{
  static auto const values = tts::bench::random<float>(-1.f, 1.f);
  return values;
}

TTS_CASE_TPL("Benchmark std::sin against its cubic Taylor polynomial", float, double)
<typename T>(tts::type<T>)
{
  auto const x = tts::bench::convert<T>(inputs());

  // One table per type, titled with the name of the type
  tts::bench::benchmark b(tts::text("sin in ") + tts::as_text(tts::typename_<T>));
  TTS_RUN_BENCHMARK(b, T, "std::sin", [](T v) { return std::sin(v); }, x);
  TTS_RUN_BENCHMARK(b, T, "polynomial", [](T v) { return v - v * v * v / 6; }, x);

  TTS_PASS("Benchmarked");
};
//! [snippet]
