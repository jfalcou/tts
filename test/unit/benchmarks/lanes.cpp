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
#include <array>
#include <cmath>
#include <concepts>

using pack = std::array<float, 4>;

// A row of type pack has four lanes: its function takes the float buffer four values at a time
struct packed
{
  template<typename Row> static constexpr std::size_t lanes = std::same_as<Row, pack> ? 4 : 1;

  template<typename Row, typename Value>
  static decltype(auto) input(tts::buffer<Value> const& values)
  {
    if constexpr(std::same_as<Row, pack>)
    {
      tts::buffer<pack> packs;
      for(std::size_t i = 0; i + 3 < values.size(); i += 4)
        packs.push_back({values[ i ], values[ i + 1 ], values[ i + 2 ], values[ i + 3 ]});
      return packs;
    }
    else return tts::bench::default_traits::input<Row>(values);
  }

  template<typename Result> static void prevent_optimization(Result const& result)
  {
    tts::bench::default_traits::prevent_optimization(result);
  }

  static tts::text environment()
  {
    return tts::text("Four floats per pack");
  }
};

pack roots(pack const& v)
{
  return {std::sqrt(v[ 0 ]), std::sqrt(v[ 1 ]), std::sqrt(v[ 2 ]), std::sqrt(v[ 3 ])};
}

TTS_CASE("Benchmark std::sqrt on one float and on a pack of four")
{
  auto x = tts::bench::random<float>(1.f, 100.f);

  // With the packed traits, the time of the pack row is divided by four
  tts::bench::basic_benchmark<packed> b("sqrt by lanes");
  TTS_RUN_BENCHMARK(b, float, "one float", [](float v) { return std::sqrt(v); }, x);
  TTS_RUN_BENCHMARK(b, pack, "four floats", [](pack const& v) { return roots(v); }, x);

  TTS_PASS("Benchmarked");
};
//! [snippet]
