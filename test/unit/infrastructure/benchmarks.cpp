//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/benchmarks.hpp>
#include <cmath>
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
};

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

struct four_lanes : tts::bench::default_traits
{
  template<typename Row> static constexpr std::size_t lanes = 4; // NOSONAR - traits hide by design

  static tts::text                                    environment()
  {
    return tts::text("**Machine:** test");
  }
};

template<typename Traits = tts::bench::default_traits, typename Buffer, typename... Counts>
tts::text table_of(char const* title, Buffer const& x, Counts... counts)
{
  auto           twice = [](int v) { return 2 * v; };

  recording_sink sink;
  {
    tts::scoped_sink                    scope(sink);
    tts::bench::basic_benchmark<Traits> b(title, static_cast<std::size_t>(counts)...);
    TTS_RUN_BENCHMARK(b, int, "twice", twice, x);
  }
  return sink.all;
}

bool holds(tts::text const& output, char const* s)
{
  return std::strstr(output.data(), s) != nullptr; // NOSONAR - C++23 on a C++20 project
}

TTS_CASE("Check that tts::bench::random stays in its range and repeats under the same seed")
{
  tts::set_random_seed(42);
  auto first = tts::bench::random<int>(-20, 20);
  tts::set_random_seed(42);
  auto again = tts::bench::random<int>(-20, 20);

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

TTS_CASE("Check that tts::bench::random generates the number of values it is given")
{
  TTS_EQUAL(tts::bench::random<int>(0, 9, 10).size(), std::size_t {10});
};

TTS_CASE("Check that tts::bench::random generates both values of bool")
{
  auto        flags = tts::bench::random<bool>(false, true);

  std::size_t trues = 0;
  for(bool f: flags)
    trues += f ? 1U : 0U;

  TTS_GREATER(trues, std::size_t {0});
  TTS_LESS(trues, flags.size());
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

  TTS_EXPECT(holds(sink.all, "# Benchmarking for : `table`"));
  TTS_EXPECT(holds(sink.all, "| Name "));
  TTS_EXPECT(holds(sink.all, "| first "));
  TTS_EXPECT(holds(sink.all, "| second "));
  TTS_EXPECT(holds(
  sink.all, "|    ns/elem |        min |        max |   err% |    N |   Speedup |     Eff. |"));
  bool const counters = holds(sink.all, " bra/elem |") && holds(sink.all, " miss% |");
  bool const left_out = holds(sink.all, "the performance counters so none are displayed");
  TTS_EXPECT(counters != left_out);
};

TTS_CASE("Check that each row returns its own times and its error")
{
  auto x     = tts::bench::random<int>(1, 20);
  auto twice = [](int v) { return 2 * v; };
  auto roots = [](int v)
  {
    double r = v;
    for(int i = 0; i < 64; ++i)
      r = std::sqrt(r + i);
    return r;
  };

  recording_sink          sink;
  tts::bench::measurement light {};
  tts::bench::measurement heavy {};
  tts::bench::measurement single {};
  {
    tts::scoped_sink      scope(sink);
    tts::bench::benchmark several("several epochs", 5);
    light = TTS_RUN_BENCHMARK(several, int, "twice", twice, x);
    heavy = TTS_RUN_BENCHMARK(several, int, "roots", roots, x);

    tts::bench::benchmark one("one epoch", 1, 0);
    single = TTS_RUN_BENCHMARK(one, int, "twice", twice, x);
  }

  TTS_GREATER(light.time, 0.);
  TTS_LESS_EQUAL(light.fastest, light.time);
  TTS_LESS_EQUAL(light.time, light.slowest);
  TTS_GREATER_EQUAL(light.error, 0.);
  TTS_GREATER(heavy.time, light.time);

  TTS_EQUAL(single.fastest, single.time);
  TTS_EQUAL(single.slowest, single.time);
  TTS_EQUAL(single.error, 0.);
};

TTS_CASE("Check that a benchmark states whether the assertions of its unit are enabled")
{
  auto const output = table_of("assertions", tts::bench::random<int>(-20, 20), 3);

#if defined(NDEBUG)
  TTS_EXPECT(holds(output, "Assertions: disabled"));
#else
  TTS_EXPECT(holds(output, "Assertions: enabled"));
#endif
};

TTS_CASE("Check that a benchmark reads the lanes and the machine from its traits")
{
  auto const output = table_of<four_lanes>("traits", tts::bench::random<int>(-20, 20), 3);

  TTS_EXPECT(holds(output, "**Machine:** test"));
  TTS_EXPECT(holds(output, " |    4 |     x1.00 |"));
};

TTS_CASE("Check that a benchmark reads a std::vector under the default traits")
{
  auto const output = table_of("vector", std::vector(tts::bench::size, 3), 3);

  TTS_EXPECT(holds(output, "| twice "));
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

  tts::buffer read(32, '\0');
  TTS_EXPECT(std::fgets(read.data(), static_cast<int>(read.size()), file) != nullptr);
  TTS_EQUAL(tts::text(read.data()), tts::text("\"pow(x, \"\"3\"\")\""));
  std::fclose(file);
};

TTS_CASE("Check that a benchmark prints the epochs and the warmup of its constructor")
{
  TTS_EXPECT(
  holds(table_of("counts", tts::bench::random<int>(-20, 20), 3, 2), " - Epochs: 3 - Warmup: 2"));
};

TTS_CASE("Check that --epochs and --warmup take precedence over the constructor")
{
  scoped_env epochs("TTS_EPOCHS", "7");
  scoped_env warmup("TTS_WARMUP", "5");

  TTS_EXPECT(
  holds(table_of("override", tts::bench::random<int>(-20, 20), 3, 2), " - Epochs: 7 - Warmup: 5"));
};

TTS_CASE("Check that an even number of epochs is raised to the next odd one")
{
  TTS_EXPECT(holds(table_of("even epochs", tts::bench::random<int>(-20, 20), 4, 2),
                   " - Epochs: 5 - Warmup: 2"));
};

TTS_CASE("Check that a benchmark runs one epoch at least")
{
  scoped_env epochs("TTS_EPOCHS", "0");

  TTS_EXPECT(
  holds(table_of("one epoch", tts::bench::random<int>(-20, 20), 3, 2), " - Epochs: 1 - Warmup: 2"));
};

TTS_CASE("Check that a benchmark warms up 100 passes by default")
{
  TTS_EXPECT(holds(table_of("default warmup", tts::bench::random<int>(-20, 20), 3),
                   " - Epochs: 3 - Warmup: 100"));
};

TTS_CASE("Check that a benchmark measures 101 epochs by default")
{
  TTS_EXPECT(holds(table_of("default epochs", tts::bench::random<int>(-20, 20)),
                   " - Epochs: 101 - Warmup: 100"));
};

TTS_CASE("Check that a negative count is raised to its minimum")
{
  scoped_env epochs("TTS_EPOCHS", "-5");
  scoped_env warmup("TTS_WARMUP", "-1");

  TTS_EXPECT(holds(table_of("negative counts", tts::bench::random<int>(-20, 20), 3, 2),
                   " - Epochs: 1 - Warmup: 0"));
};

TTS_CASE("Check that an err% of 5% or more carries a mark, as in nanobench")
{
  TTS_EQUAL(tts::bench::_::spread(0.02), tts::text("2.0%"));
  TTS_EQUAL(tts::bench::_::spread(0.05), tts::text("5.0%~"));
  TTS_EQUAL(tts::bench::_::spread(0.06), tts::text("6.0%~"));
};
