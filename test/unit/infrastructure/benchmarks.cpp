//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/benchmarks.hpp>
#include <cstring>

struct recording_sink : tts::output_sink
{
  tts::text all;

  void      write(tts::text const& t) override
  {
    all += t;
  }

  bool holds(char const* s) const
  {
    return std::strstr(all.data(), s) != nullptr;
  }
};

// Reads every buffer with four lanes, and names the machine.
struct four_lanes : tts::bench::default_traits
{
  template<typename Row> static constexpr std::size_t lanes = 4;

  static tts::text                                    environment()
  {
    return tts::text("**Machine:** test");
  }
};

TTS_CASE("Check that tts::bench::random draws in its range, the same values from the same state")
{
  tts::bench::prng_state = 42;
  auto first             = tts::bench::random<int>(-20, 20);
  tts::bench::prng_state = 42;
  auto again             = tts::bench::random<int>(-20, 20);

  TTS_EQUAL(first.size(), tts::bench::size);

  bool in_range = true, same = true;
  for(std::size_t i = 0; i < first.size(); ++i)
  {
    in_range = in_range && first[ i ] >= -20 && first[ i ] <= 20;
    same     = same && first[ i ] == again[ i ];
  }
  TTS_EXPECT(in_range);
  TTS_EXPECT(same);
};

TTS_CASE("Check that a draw of tts::bench::random leaves the generator of the run untouched")
{
  auto const            run    = tts::_::prng_state;
  [[maybe_unused]] auto values = tts::bench::random<double>(0., 1.);
  TTS_EQUAL(tts::_::prng_state, run);
};

TTS_CASE("Check that tts::bench::convert converts every value of its buffer")
{
  auto values    = tts::bench::random<int>(-20, 20);
  auto converted = tts::bench::convert<double>(values);

  TTS_EQUAL(converted.size(), values.size());

  bool same = true;
  for(std::size_t i = 0; i < values.size(); ++i)
    same = same && converted[ i ] == static_cast<double>(values[ i ]);
  TTS_EXPECT(same);
};

TTS_CASE("Check that a benchmark reports one row per run under its name")
{
  auto           x     = tts::bench::random<int>(-20, 20);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark b("table", 3);
    TTS_RUN_BENCHMARK(b, int, "first", twice, x);
    TTS_RUN_BENCHMARK(b, int, "second", twice, x);
  }

  TTS_EXPECT(sink.holds("# Benchmarking for : `table`"));
  TTS_EXPECT(sink.holds("| Name "));
  TTS_EXPECT(sink.holds("| first "));
  TTS_EXPECT(sink.holds("| second "));
};

TTS_CASE("Check that a benchmark reads the lanes and the machine from its traits")
{
  auto           x     = tts::bench::random<int>(-20, 20);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink                        scope(sink);
    tts::bench::basic_benchmark<four_lanes> b("traits", 3);
    TTS_RUN_BENCHMARK(b, int, "wide", twice, x);
  }

  TTS_EXPECT(sink.holds("**Machine:** test"));
  TTS_EXPECT(sink.holds("| wide |    4 |"));
};
