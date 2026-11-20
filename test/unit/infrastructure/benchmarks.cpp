//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/benchmarks.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

struct recording_sink : tts::output_sink
{
  tts::text all;

  void      write(tts::text const& t) override
  {
    all += t;
  }

  bool holds(char const* s) const
  {
    return std::strstr(all.data(), s) != nullptr; // NOSONAR - C++23 on a C++20 project
  }
};

// Sets an environment variable for its lifetime, which the options read as --name=value.
struct scoped_env
{
  char const* name;

  scoped_env(char const* n, char const* value)
      : name(n)
  {
#if defined(_WIN32)
    _putenv_s(name, value);
#else
    setenv(name, value, 1);
#endif
  }

  scoped_env(scoped_env const&)            = delete;
  scoped_env& operator=(scoped_env const&) = delete;

  ~scoped_env()
  {
#if defined(_WIN32)
    _putenv_s(name, "");
#else
    unsetenv(name);
#endif
  }
};

// Reads every buffer with four lanes, and names the machine.
struct four_lanes : tts::bench::default_traits
{
  template<typename Row> static constexpr std::size_t lanes = 4; // NOSONAR - traits hide by design

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

  bool in_range = true;
  bool same     = true;
  for(std::size_t i = 0; i < first.size(); ++i)
  {
    in_range = in_range && first[ i ] >= -20 && first[ i ] <= 20;
    same     = same && first[ i ] == again[ i ];
  }
  TTS_EXPECT(in_range);
  TTS_EXPECT(same);
};

TTS_CASE("Check that tts::bench::random draws the number of values it is given")
{
  TTS_EQUAL(tts::bench::random<int>(0, 9, 10).size(), std::size_t {10});
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
  TTS_EXPECT(sink.holds(" ns/elem |"));
  TTS_EXPECT(sink.holds(" IPC |"));
  TTS_EXPECT(sink.holds(" bra/elem |"));
  TTS_EXPECT(sink.holds(" miss% |"));
};

TTS_CASE("Check that a benchmark states whether the assertions of its unit are enabled")
{
  auto           x     = tts::bench::random<int>(-20, 20);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark b("assertions", 3);
    TTS_RUN_BENCHMARK(b, int, "twice", twice, x);
  }

#if defined(NDEBUG)
  TTS_EXPECT(sink.holds("Assertions: disabled"));
#else
  TTS_EXPECT(sink.holds("Assertions: enabled"));
#endif
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

TTS_CASE("Check that a benchmark reads a std::vector under the default traits")
{
  std::vector    x(tts::bench::size, 3);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark b("vector", 3);
    TTS_RUN_BENCHMARK(b, int, "from a vector", twice, x);
  }

  TTS_EXPECT(sink.holds("| from a vector "));
};

TTS_CASE("Check that the table prints three significant digits")
{
  TTS_EQUAL(tts::bench::_::digits(1.), tts::text("1.00"));
  TTS_EQUAL(tts::bench::_::digits(24.13), tts::text("24.1"));
  TTS_EQUAL(tts::bench::_::digits(0.0412), tts::text("0.0412"));
  TTS_EQUAL(tts::bench::_::digits(7140.), tts::text("7140"));
};

TTS_CASE("Check that the CSV quotes a name and doubles its quotes")
{
  FILE* file = std::tmpfile();
  TTS_EXPECT(file != nullptr);

  tts::bench::_::quoted(file, "pow(x, \"3\")");
  std::rewind(file);

  tts::buffer<char> read(32, '\0');
  TTS_EXPECT(std::fgets(read.data(), static_cast<int>(read.size()), file) != nullptr);
  TTS_EQUAL(tts::text(read.data()), tts::text("\"pow(x, \"\"3\"\")\""));
  std::fclose(file);
};

TTS_CASE("Check that a benchmark prints the epochs and the warmup of its constructor")
{
  auto           x     = tts::bench::random<int>(-20, 20);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark b("counts", 3, 2);
    TTS_RUN_BENCHMARK(b, int, "twice", twice, x);
  }

  TTS_EXPECT(sink.holds(" - Epochs: 3 - Warmup: 2"));
};

TTS_CASE("Check that --epochs and --warmup take precedence over the constructor")
{
  auto           x     = tts::bench::random<int>(-20, 20);
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    scoped_env            epochs("TTS_EPOCHS", "4");
    scoped_env            warmup("TTS_WARMUP", "5");
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark b("override", 3, 2);
    TTS_RUN_BENCHMARK(b, int, "twice", twice, x);
  }

  TTS_EXPECT(sink.holds(" - Epochs: 4 - Warmup: 5"));
};
