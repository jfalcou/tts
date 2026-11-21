//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

// Do not include this header from tts.hpp: nanobench pulls <chrono>, <string>, and <vector> into
// every unit that reads it, and the standalone header follows the includes of tts.hpp.
#include <tts/tts.hpp>
#include <tts/tools/file.hpp>

#if defined(TTS_MAIN) && !defined(ANKERL_NANOBENCH_IMPLEMENT)
#define ANKERL_NANOBENCH_IMPLEMENT
#endif
#include <nanobench.h>

#include <cstdint>
#include <cstdio>
#include <type_traits>
#include <utility>

//======================================================================================================================
/**
  @defgroup tools-benchmarks Benchmark Utilities
  @brief Tools to time functions with [nanobench](https://nanobench.ankerl.com) and print their
  times and throughput as Markdown tables.

  `tts/benchmarks.hpp` is a separate header, not included by `tts/tts.hpp` or by the standalone
  header. This header includes `nanobench.h`, which the CMake option `TTS_BENCHMARKS` provides
  through the `tts::benchmarks` target (see @ref setup-benchmarks). The implementation of
  [nanobench](https://nanobench.ankerl.com) is compiled in the translation unit that defines
  @ref TTS_MAIN, and no other macro needs to be defined.

  A benchmark is a TTS case that draws its inputs once (usually through `tts::bench::random`), then
  measures each function as a table row with @ref TTS_RUN_BENCHMARK. The table is printed when
  the benchmark goes out of scope, and its first row is the baseline of every speedup. Such a case
  records no check, so it will not count as a success unless it ends with @ref TTS_PASS.

  @groupheader{Measurement}

  Each row calls its function once per index of its buffers, and the `i`-th call receives the `i`-th
  value of every buffer. If the buffers differ in length, the shortest buffer determines the number
  of calls. A lambda or a function object can be inlined in the timed loop, but a function name is
  called through a pointer, so every call is indirect.

  An epoch is a sequence of complete passes over all buffers, whose number
  [nanobench](https://nanobench.ankerl.com) chooses so that the epoch lasts at least 1 ms. By
  default, each row runs 100 epochs after a single warmup pass. Both counts are parameters of the
  benchmark constructor (the epochs must be at least one), and `--epochs=N` and `--warmup=N`
  override them for an entire executable. A sink follows every call to keep its result alive, so the
  measured time includes the loop, the loads, and the sink, and consecutive calls cannot be
  vectorized. The calls are independent and the buffers stay in cache, so a row measures throughput
  on hot data, not latency.

  @groupheader{Table}

  A line under the title, such as `Assertions: disabled - Epochs: 100 - Warmup: 1`, tells whether
  assertions are enabled in the translation unit (from `NDEBUG`) and gives both counts. An element
  is a single value, so a call of a row with `N` lanes processes `N` elements. The table has one row
  per function, with these columns:

  | Column | Content |
  |---|---|
  | `N` | values processed per call, from the traits |
  | `Speedup` | elements per second of the row over those of the first row |
  | `Eff.` | the speedup over `N` in percent, `-` on a single-lane row after the first |
  | `ns/elem`, `min`, `max` | median, smallest, and largest time per element, in nanoseconds |
  | `err%` | median absolute percentage error of the epoch times |
  | `elem/s` | elements per second, the inverse of the median time per element |
  | `cyc/elem` | median CPU cycles per element |
  | `ins/elem` | median instructions per element |
  | `IPC` | instructions per cycle, median instructions over median cycles |
  | `bra/elem` | median branch instructions per element |
  | `miss%` | median mispredicted branches over median branch instructions, in percent |

  `err%` is the median of `|t - m| / t` over the epochs, where `t` is the duration of an epoch and
  `m` is the median duration. If the operating system blocks the hardware counters (as many
  containers do), their five columns read `-`, and a note under the table says so.

  If the executable is run with `-d` or `--dump`, it also writes `<title>.csv` in the working
  directory, with:

  - one quoted column name per row.
  - one line per epoch.
  - each epoch's time per element, in seconds.

  If this file cannot be opened (for instance, because the title contains a `/`), a note under the
  table title reports the failure.

  @groupheader{Inputs}

  The inputs of a row are buffers, one per parameter of its function.

  - `tts::bench::random` returns a `tts::buffer`, the vector type of TTS, which provides `size()`,
    `operator[]`, and `push_back`, and supports range-based for loops.
  - The number of values drawn is the third argument of `tts::bench::random`, which is
    `tts::bench::size` if not specified.
  - The draws come from a dedicated generator, so they depend neither on the seed that TTS prints
    at startup nor on `--seed`.
  - Under `tts::bench::default_traits`, a row:
    - reads its buffers unchanged, a `tts::buffer` or any other container with `size()` and
      `operator[]`, such as `std::vector`.
    - has `N` set to 1, whatever its type.

  @groupheader{Example}
  @snippet benchmarks/hypot.cpp snippet
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_RUN_BENCHMARK
  @brief Times a benchmark row.

  @param RUNNER Benchmark receiving the row.
  @param TYPE   Type of the row, used by the traits to build its input and to count its lanes.
                Under `tts::bench::default_traits`, this type only sets `N` to 1.
  @param NAME   Name of the row, as printed in the table.
  @param FUNC   Function to time, called once per index of the buffers. A lambda or a function
                object should be preferred, since the compiler can then inline each call.
  @param ...    Buffers holding the arguments for `FUNC`, in parameter order. The `i`-th call
                takes the `i`-th value of each buffer.

  @groupheader{Example}
  @snippet benchmarks/hypot.cpp snippet
**/
//======================================================================================================================
#define TTS_RUN_BENCHMARK(RUNNER, TYPE, NAME, FUNC, ...)                                           \
  (RUNNER).run<TYPE>(NAME, FUNC, __VA_ARGS__) /**/

//======================================================================================================================
/**
  @def TTS_RUN_BENCHMARK_TPL
  @brief Times a benchmark row whose type depends on a template parameter.

  This macro inserts the `.template` keyword that a benchmark of dependent type requires. Its
  parameters are those of @ref TTS_RUN_BENCHMARK.

  @groupheader{Example}
  @snippet benchmarks/hypot.cpp snippet
**/
//======================================================================================================================
#define TTS_RUN_BENCHMARK_TPL(RUNNER, TYPE, NAME, FUNC, ...)                                       \
  (RUNNER).template run<TYPE>(NAME, FUNC, __VA_ARGS__) /**/

namespace tts::bench
{
  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief The default number of values in each buffer that `tts::bench::random` draws (a
           multiple of common SIMD register widths).

    @groupheader{Example}
    @snippet benchmarks/draws.cpp snippet
  **/
  //======================================================================================================================
  inline constexpr std::size_t size = 1024;

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief The state of the generator that draws every benchmark buffer.

    Its initial value is a fixed seed, so two executions will time the same values. This generator
    is independent from the generator used by the test run, so `--seed` and `tts::set_random_seed`
  do not affect its draws. Assigning a new state changes the values of the following draws.

    @groupheader{Example}
    @snippet benchmarks/draws.cpp snippet
  **/
  //======================================================================================================================
  inline std::uint64_t prng_state = 0x853c49e6748fea9bULL; // NOSONAR - a draw advances it

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Draws a value uniformly on [mini, maxi] with the benchmark generator, as
           `tts::bench::random` does for each element.

    @tparam T   Arithmetic type of the value.
    @param mini Inclusive lower bound of the drawn range.
    @param maxi Inclusive upper bound of the drawn range.
    @return A value of type `T` between `mini` and `maxi`.

    @groupheader{Example}
    @snippet benchmarks/draws.cpp snippet
  **/
  //======================================================================================================================
  // Do not draw from tts::_::prng_state as it stands: the run seeds it from the clock, and two
  // executions would time different values.
  template<typename T> T roll(T mini, T maxi)
  {
    std::uint64_t const run   = std::exchange(tts::_::prng_state, prng_state);
    T const             value = tts::_::roll(mini, maxi);
    prng_state                = std::exchange(tts::_::prng_state, run);
    return value;
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Draws a buffer of `count` values uniformly distributed over [mini, maxi].

    Floating-point values are drawn uniformly, whereas `tts::random_value` draws them
    log-uniformly.

    @tparam T    Arithmetic type of the values.
    @param mini  Inclusive lower bound of the drawn range.
    @param maxi  Inclusive upper bound of the drawn range.
    @param count Number of values to draw (`tts::bench::size` if not specified).
    @return A `tts::buffer` holding `count` values of type `T`.

    @groupheader{Example}
    @snippet benchmarks/draws.cpp snippet
  **/
  //======================================================================================================================
  template<typename T>
    requires(std::is_arithmetic_v<T>)
  tts::buffer<T> random(T mini, T maxi, std::size_t count = size)
  {
    tts::buffer<T> values(count);
    for(auto& v: values)
      v = roll(mini, maxi);
    return values;
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Converts every value of a buffer to `U`, so that a row can time the same values in
           another type.

    @tparam U     Type to which every value is converted.
    @param values Buffer to convert (not modified).
    @return A `tts::buffer` holding `U(v)` for every value `v` of `values`.

    @groupheader{Example}
    @snippet benchmarks/traits.cpp snippet
  **/
  //======================================================================================================================
  template<typename U, typename T> tts::buffer<U> convert(tts::buffer<T> const& values)
  {
    tts::buffer<U> out;
    for(auto const& v: values)
      out.push_back(U(v));
    return out;
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Traits under which a row reads its buffers as drawn and processes a single value per
           call.

    `tts::bench::basic_benchmark` uses four members of its traits. A library may derive from
    `default_traits` to replace only the members it needs.

    - `input<Row>(values)` builds the input of a row from a buffer.
    - `lanes<Row>` is the number of values that a call processes.
    - `sink(result)` keeps a result from being optimized away.
    - `environment()` returns a line printed under the table title.

    @groupheader{Example}
    @snippet benchmarks/traits.cpp snippet
  **/
  //======================================================================================================================
  struct default_traits
  {
    //======================================================================================================================
    /**
      @brief Returns `values` unchanged, as the buffer read by a row of type `Row`.

      `values` may be any buffer with `size()` and `operator[]`.
    **/
    //======================================================================================================================
    template<typename Row, typename Buffer> static Buffer const& input(Buffer const& values)
    {
      return values;
    }

    //======================================================================================================================
    /**
      @brief The number of values that each call processes for a row of type `Row` (1 under the
             default traits).
    **/
    //======================================================================================================================
    template<typename Row> static constexpr std::size_t lanes = 1;

    //======================================================================================================================
    /**
      @brief Keeps a result from being optimized away, with
             [`ankerl::nanobench::doNotOptimizeAway`](https://nanobench.ankerl.com/reference.html).
    **/
    //======================================================================================================================
    template<typename Result> static void sink(Result const& result)
    {
      ankerl::nanobench::doNotOptimizeAway(result); // NOSONAR - names this parameter itself
    }

    //======================================================================================================================
    /**
      @brief Returns an empty text, so no line is printed under the table title.
    **/
    //======================================================================================================================
    static tts::text environment()
    {
      return tts::text {};
    }
  };

  namespace _
  {
    // Do not print with %.3g instead: it writes 7140 as 7.14e+03.
    inline tts::text digits(double v)
    {
      int    decimals = 2;
      double bound    = 10.;
      for(int k = 0; k < 2; ++k)
      {
        if(v < bound) break;
        --decimals;
        bound *= 10.;
      }

      bound = 1.;
      for(int k = 0; k < 7; ++k)
      {
        if(v <= 0. || v >= bound) break;
        ++decimals;
        bound /= 10.;
      }
      return tts::text("%.*f", decimals, v);
    }

    inline void quoted(FILE* file, char const* name)
    {
      std::fputc('"', file);
      for(char const* c = name; *c; ++c)
      {
        if(*c == '"') std::fputc('"', file);
        std::fputc(*c, file);
      }
      std::fputc('"', file);
    }
  }

  TTS_DISABLE_WARNING_PUSH
  TTS_DISABLE_WARNING_CRT_SECURE

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Times the rows of a table with [nanobench](https://nanobench.ankerl.com) and prints this
           table when the benchmark goes out of scope.

    @tparam Traits Traits of the rows (`tts::bench::default_traits` if not specified).

    @groupheader{Example}
    @snippet benchmarks/traits.cpp snippet
  **/
  //======================================================================================================================
  template<typename Traits = default_traits> class basic_benchmark
  {
  private:
    ankerl::nanobench::Bench    bench_;
    std::size_t                 repetitions_;
    std::size_t                 warmup_;

    tts::buffer<std::ptrdiff_t> run_cardinals_;
    tts::text                   base_file_name_;

    void                        configure_bench()
    {
      bench_.output(nullptr);
      bench_.performanceCounters(true);
      bench_.epochs(
      tts::_::max(std::size_t {1}, ::tts::arguments().value(repetitions_, "--epochs")));
      bench_.warmup(::tts::arguments().value(warmup_, "--warmup"));
    }

    template<typename Row, typename Func, typename... Inputs> // NOSONAR - <iterator> is refused
    void time_run(tts::text const& name, Func& func, Inputs const&... in)
    {
      constexpr std::size_t N = Traits::template lanes<Row>;
      run_cardinals_.push_back(static_cast<std::ptrdiff_t>(N));

      auto count = static_cast<std::size_t>(-1);
      ((count = tts::_::min(count, in.size())), ...);
      bench_.batch(count * N);

      bench_.run(name.data(),
                 [ &count, &func, &in... ]()
                 {
                   for(std::size_t i = 0; i < count; ++i)
                   {
                     // Do not write auto: it would copy a result returned by reference, and such a
                     // copy can allocate.
                     decltype(auto) res = func(in[ i ]...);
                     Traits::sink(res);
                   }
                 });
    }

    template<typename Row, typename Func, typename... Values>
    void execute_single_run(tts::text const& name, Func& func, Values const&... values)
    {
      time_run<Row>(name, func, Traits::template input<Row>(values)...);
    }

    static void dashes(tts::text& line, int count)
    {
      while(count-- > 0)
        line += "-";
    }

    // Do not divide by the cardinal of the run: an epoch times one call of the lambda, a pass over
    // the batch.
    template<typename Results>
    static void write_epoch(FILE* file, Results const& results, std::size_t e)
    {
      for(std::size_t r = 0; r < results.size(); ++r)
      {
        if(auto const& res = results[ r ]; e < res.size())
        {
          double const elapsed = res.get(e, ankerl::nanobench::Result::Measure::elapsed);
          tts::text    val {"%g", elapsed / static_cast<double>(res.config().mBatch)};
          std::fputs(val.data(), file);
        }
        if(r + 1 < results.size()) std::fputs(",", file);
      }
      std::fputs("\n", file);
    }

    // One CSV column per run and one line per epoch, each time divided by the number of elements of
    // its run.
    template<typename Results> tts::text dump_csv(Results const& results) const
    {
      tts::text          filename = base_file_name_ + ".csv";
      tts::_::file_guard out_file(std::fopen(filename.data(), "w"));
      if(!out_file)
        return tts::text("> Note: %s could not be opened for writing.", filename.data());

      std::size_t epochs = 0;
      for(std::size_t r = 0; r < results.size(); ++r)
      {
        _::quoted(out_file.get(), results[ r ].config().mBenchmarkName.c_str());
        std::fputs(r + 1 < results.size() ? "," : "\n", out_file.get());
        epochs = tts::_::max(epochs, results[ r ].size());
      }

      for(std::size_t e = 0; e < epochs; ++e)
        write_epoch(out_file.get(), results, e);
      return tts::text {};
    }

    template<typename Out>
    static void print_row(Out&                          out,
                          std::size_t                   name_width,
                          char const*                   name,
                          tts::buffer<tts::text> const& cells,
                          tts::buffer<int> const&       widths)
    {
      tts::text line("| ");
      line += tts::text("%-*s", static_cast<int>(name_width), name);
      for(std::size_t c = 0; c < cells.size(); ++c)
        line += tts::text(" | %*s", widths[ c ], cells[ c ].data());
      line += " |";
      out.writeln(line);
    }

    template<typename Result>
    tts::buffer<tts::text>
    row_cells(Result const& res, std::size_t i, double baseline_elem_per_sec) const
    {
      using measure               = ankerl::nanobench::Result::Measure;
      auto const     batch_size   = static_cast<double>(res.config().mBatch);
      std::ptrdiff_t N            = run_cardinals_[ i ];
      double const   current_time = res.median(measure::elapsed);
      double const   elem_per_sec = batch_size / current_time;
      double const   speedup      = elem_per_sec / baseline_elem_per_sec;
      double const   efficiency   = speedup / static_cast<double>(N);
      double const   instructions = res.median(measure::instructions);
      double const   cycles       = res.median(measure::cpucycles);
      double const   branches     = res.median(measure::branchinstructions);
      bool const     counted      = cycles > 0.0 || instructions > 0.0;

      return {tts::text("%td", N),
              tts::text("x") + _::digits(speedup),
              (N == 1 && i != 0) ? tts::text("-") : tts::text("%.1f%%", efficiency * 100.0),
              _::digits(1e9 * current_time / batch_size),
              _::digits(1e9 * res.minimum(measure::elapsed) / batch_size),
              _::digits(1e9 * res.maximum(measure::elapsed) / batch_size),
              tts::text("%.1f%%", res.medianAbsolutePercentError(measure::elapsed) * 100.0),
              tts::text("%.0f", elem_per_sec),
              counted ? tts::text("%.2f", cycles / batch_size) : tts::text("-"),
              counted ? tts::text("%.2f", instructions / batch_size) : tts::text("-"),
              cycles > 0.0 ? tts::text("%.2f", instructions / cycles) : tts::text("-"),
              counted ? tts::text("%.2f", branches / batch_size) : tts::text("-"),
              branches > 0.0
              ? tts::text("%.1f%%", 100.0 * res.median(measure::branchmisses) / branches)
              : tts::text("-")};
    }

    template<typename Results> void print_table(Results const& results) const
    {
      auto&       out        = tts::output();
      std::size_t name_width = 4;
      for(auto const& res: results)
        name_width = tts::_::max(name_width, res.config().mBenchmarkName.size());

      tts::buffer<char const*> const headers = {"N",
                                                "Speedup",
                                                "Eff.",
                                                "ns/elem",
                                                "min",
                                                "max",
                                                "err%",
                                                "elem/s",
                                                "cyc/elem",
                                                "ins/elem",
                                                "IPC",
                                                "bra/elem",
                                                "miss%"};
      tts::buffer const              widths  = {4, 9, 8, 10, 10, 10, 6, 12, 10, 10, 6, 10, 6};

      tts::buffer<tts::text>         header_cells;
      for(char const* h: headers)
        header_cells.push_back(tts::text(h));
      print_row(out, name_width, "Name", header_cells, widths);

      tts::text separator("|:");
      dashes(separator, static_cast<int>(name_width));
      separator += "-|";
      for(int w: widths)
      {
        dashes(separator, w + 1);
        separator += ":|";
      }
      out.writeln(separator);

      using measure     = ankerl::nanobench::Result::Measure;
      auto const& first = results.front();
      auto const  baseline =
      static_cast<double>(first.config().mBatch) / first.median(measure::elapsed);

      bool counters_failed = false;
      for(std::size_t i = 0; i < results.size(); ++i)
      {
        auto const& res = results[ i ];
        if(res.median(measure::cpucycles) == 0.0 && res.median(measure::instructions) == 0.0)
          counters_failed = true;
        print_row(
        out, name_width, res.config().mBenchmarkName.c_str(), row_cells(res, i, baseline), widths);
      }

      if(counters_failed)
      {
        out.writeln();
        out.writeln("> Note: the OS blocks the hardware counters, so their five columns read -.");
      }
    }

  public:
    //======================================================================================================================
    /**
      @brief Builds a benchmark whose table is titled `benchmark`.

      Both counts can be overridden with `--epochs=N` and `--warmup=N`.

      @param repetitions Number of epochs for each row, at least 1 (100 if not specified).
      @param warmup      Number of warmup passes over all buffers (1 if not specified).
    **/
    //======================================================================================================================
    explicit basic_benchmark(std::size_t repetitions = 100, std::size_t warmup = 1)
        : repetitions_(repetitions)
        , warmup_(warmup)
        , base_file_name_("benchmark")
    {
      configure_bench();
    }

    //======================================================================================================================
    /**
      @brief Builds a benchmark whose table is titled `name`.

      Both counts can be overridden with `--epochs=N` and `--warmup=N`.

      @param name        Title of the table, also the base name of its CSV file.
      @param repetitions Number of epochs for each row, at least 1 (100 if not specified).
      @param warmup      Number of warmup passes over all buffers (1 if not specified).
    **/
    //======================================================================================================================
    // Do not drop the constraint: an integer would select this constructor and be taken as a name.
    template<typename Name>
    explicit basic_benchmark(Name const& name,
                             std::size_t repetitions = 100,
                             std::size_t warmup      = 1)
      requires(!std::is_integral_v<Name>)
        : repetitions_(repetitions)
        , warmup_(warmup)
        , base_file_name_(tts::text(name))
    {
      configure_bench();
    }

    basic_benchmark(basic_benchmark const&)            = delete;
    basic_benchmark& operator=(basic_benchmark const&) = delete;

    //======================================================================================================================
    /**
      @brief Prints the table with `tts::bench::basic_benchmark::report`.
    **/
    //======================================================================================================================
    ~basic_benchmark()
    {
      report();
    }

    //======================================================================================================================
    /**
      @brief Drops the rows timed so far, so that the destructor prints no table for them.

      @return A reference to the benchmark, to allow chained calls.
    **/
    //======================================================================================================================
    basic_benchmark& reset()
    {
      bench_         = ankerl::nanobench::Bench();
      run_cardinals_ = tts::buffer<std::ptrdiff_t>();
      configure_bench();
      return *this;
    }

    //======================================================================================================================
    /**
      @brief Times a row of type `Row` (the function called by @ref TTS_RUN_BENCHMARK).

      @tparam Row   Type of the row, used by the traits to build its input and to count its
                    lanes.
      @param name   Name of the row, as printed in the table.
      @param func   Function to time, called once per index of the buffers.
      @param values Buffers holding the arguments for `func`, in parameter order.
      @return A reference to the benchmark, to allow chained calls.
    **/
    //======================================================================================================================
    template<typename Row, typename Name, typename Func, typename... Values>
    basic_benchmark& run(Name const& name, Func&& func, Values const&... values)
    {
      execute_single_run<Row>(tts::text(name), func, values...);
      return *this;
    }

    //======================================================================================================================
    /**
      @brief Prints the table of the rows timed so far, and writes the CSV file if `-d` or `--dump`
             is given.

      The destructor calls this function as well, so a call by hand that is followed by
      `tts::bench::basic_benchmark::reset` prints the table only once.

      @return A reference to the benchmark, to allow chained calls.
    **/
    //======================================================================================================================
    basic_benchmark& report()
    {
      auto const& results = bench_.results();
      if(results.empty()) return *this;

      tts::text dump_note;
      if(::tts::arguments()("-d", "--dump")) dump_note = dump_csv(results);

      auto& out = tts::output();
      out.writeln("# Benchmarking for : `%s`", base_file_name_.data());
#if defined(NDEBUG)
      char const* assertions = "disabled";
#else
      char const* assertions = "enabled";
#endif
      out.writeln("Assertions: %s - Epochs: %zu - Warmup: %llu",
                  assertions,
                  bench_.epochs(),
                  static_cast<unsigned long long>(bench_.warmup()));
      if(tts::text const env = Traits::environment(); !env.is_empty()) out.writeln(env);
      if(!dump_note.is_empty()) out.writeln(dump_note);
      out.writeln();

      print_table(results);

      out.writeln();
      out.flush();
      return *this;
    }
  };

  TTS_DISABLE_WARNING_POP

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief A `tts::bench::basic_benchmark` on `tts::bench::default_traits`.

    @groupheader{Example}
    @snippet benchmarks/hypot.cpp snippet
  **/
  //======================================================================================================================
  using benchmark = basic_benchmark<default_traits>;
}

/// @}
