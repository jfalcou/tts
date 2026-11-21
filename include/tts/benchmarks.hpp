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
  measures each function as a table row through @ref TTS_RUN_BENCHMARK. The table is printed when
  the benchmark goes out of scope, and its first row is the baseline of every speedup. Two
  benchmarks in one scope print in reverse construction order. Such a case records no check, so TTS
  reports it as invalid unless it ends with @ref TTS_PASS.

  @groupheader{Measurement}

  Each row calls its function once per index of its buffers, and the `i`-th call receives the `i`-th
  value of every buffer. If the buffers differ in length, the shortest buffer determines the number
  of calls. Lambdas and function objects can be inlined in the timed loop, but function names are
  called through pointers, making every call indirect.

  Each row is timed when its @ref TTS_RUN_BENCHMARK runs, after its traits have built its inputs
  outside the timing. An epoch is a sequence of complete passes over all buffers, and
  [nanobench](https://nanobench.ankerl.com) calibrates the passes per epoch for each row, so that an
  epoch lasts at least 1 ms, the default of nanobench. By default, each row runs 100 epochs after
  100 warmup passes. Both counts are the second and third parameters of the benchmark constructor,
  and `--epochs=N` and `--warmup=N` override them for an entire executable. A count below its
  minimum (one epoch, zero warmup passes) is raised to that minimum. Every call is followed by a
  sink, to keep its result alive. The measured time therefore includes the loop, the loads, and the
  sink, and consecutive calls cannot be vectorized. Since the calls are independent and the buffers
  stay in cache, a row measures throughput on hot data, not latency.

  @groupheader{Table}

  Under the title line, which names the benchmark, a line such as `Assertions: disabled - Epochs:
  100 - Warmup: 100` tells whether assertions are enabled in the translation unit (from `NDEBUG`)
  and gives both counts. An element is a single value, and each call of a row with `N` lanes
  processes `N` elements. The table has one row per function, with these columns:

  | Column | Content |
  |---|---|
  | `N` | values processed per call, from the traits |
  | `Speedup` | elements per second over those of the first row, the rows timed in turn |
  | `Eff.` | the speedup over `N` in percent, `-` on a single-lane row after the first |
  | `ns/elem`, `min`, `max` | median, smallest, and largest time per element, in nanoseconds |
  | `err%` | median absolute percentage error of the epoch times, with `~` above 5% |
  | `elem/s` | elements per second, the inverse of the median time per element |
  | `cyc/elem` | median CPU cycles per element |
  | `ins/elem` | median instructions per element |
  | `bra/elem` | median branch instructions per element |
  | `miss%` | median mispredicted branches over median branch instructions, in percent |

  `err%` is the median of `|t - m| / t` over the epochs, where `t` is the time per element in an
  epoch and `m` is their median. If the operating system blocks the hardware counters (as many
  containers do), their four columns read `-`, and a note under the table says so. Above 5%, the
  threshold of nanobench for an unstable measure, a `~` follows `err%`, and a note says so as well.

  If the executable is run with `-d` or `--dump`, it also writes `<title>.csv` in the working
  directory, replacing any earlier file of that title. The file holds:

  - a header line of quoted row names.
  - one line per epoch, holding a time per element for every row, in seconds.

  The rows are timed one after the other, so a line puts side by side epochs taken at different
  times.

  If this file cannot be opened (for instance, because the title contains a `/`), a note above the
  table reports the failure.

  @groupheader{Inputs}

  The inputs of a row are buffers, one per parameter of its function.

  - The buffer that `tts::bench::random` returns is a `tts::buffer`, the vector type of TTS, which
    provides `size()`, `operator[]`, and `push_back`, and supports range-based for loops.
  - The number of values drawn is the third argument of `tts::bench::random`, which is
    `tts::bench::size` if not specified.
  - The draws come from a dedicated generator at a fixed seed, so every run draws the same values,
    whatever the seed that TTS prints at startup and `--seed`.
  - The cases of an executable draw from this generator in turn, so the inputs of a case depend on
    the cases that ran before it, under `--shard` as well.
  - A buffer drawn once can be reused by several benchmarks.
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

  The macro writes the `.template` keyword, so it also serves a benchmark whose type depends on a
  template parameter.

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

  namespace _
  {
    inline std::uint64_t prng_state = 0x853c49e6748fea9bULL; // NOSONAR - a draw advances it
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Draws a value uniformly on [mini, maxi] with the benchmark generator, as
           `tts::bench::random` does for each element.

    @tparam T   Arithmetic type of the value, `bool` included.
    @param mini Inclusive lower bound of the drawn range.
    @param maxi Inclusive upper bound of the drawn range.
    @return A value of type `T` between `mini` and `maxi`.

    @groupheader{Example}
    @snippet benchmarks/draws.cpp snippet
  **/
  //======================================================================================================================
  // Do not draw from tts::_::prng_state as it stands: the run seeds it from the clock, and two
  // executions would time different values.
  template<typename T>
    requires(std::is_arithmetic_v<T>)
  T roll(T mini, T maxi)
  {
    std::uint64_t const run   = std::exchange(tts::_::prng_state, _::prng_state);
    T const             value = tts::_::roll(mini, maxi);
    _::prng_state             = std::exchange(tts::_::prng_state, run);
    return value;
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Draws a buffer of `count` values uniformly distributed over [mini, maxi].

    Floating-point values are drawn uniformly, whereas `tts::random_value` draws them
    log-uniformly, and a buffer of `bool` drawn on [false, true] holds both values with equal
    odds.

    @tparam T    Arithmetic type of the values, `bool` included.
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

    `tts::bench::basic_benchmark` uses four members of its traits, and builds the input of a row
    once, before its timing. A library may derive from `default_traits` to replace only the
    members it needs, `lanes` by a variable template of its own,
    `template<typename Row> static constexpr std::size_t lanes = 4;`.

    - `input<Row>(values)` builds the input of a row from a buffer, by value or by reference,
      and that input lives until the row is timed.
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

      `values` may be any buffer with <code>%size()</code> and `operator[]`.
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
      @brief Keeps a result from being optimized away, through
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
      int decimals = 2;
      if(v >= 100.) decimals = 0;
      else if(v >= 10.) decimals = 1;

      double bound = 1.;
      for(int k = 0; k < 7; ++k)
      {
        if(v <= 0. || v >= bound) break;
        ++decimals;
        bound /= 10.;
      }
      return tts::text("%.*f", decimals, v);
    }

    // nanobench marks a measure above 5% as unstable, and the table uses the same threshold.
    inline tts::text spread(double error)
    {
      tts::text cell("%.1f%%", error * 100.0);
      if(error > 0.05) cell += "~";
      return cell;
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

    @tparam Traits Traits of the rows.

    @groupheader{Example}
    @snippet benchmarks/traits.cpp snippet
  **/
  //======================================================================================================================
  template<typename Traits> class basic_benchmark
  {
  private:
    static constexpr std::size_t columns            = 12;
    static constexpr char const* headers[ columns ] = {// NOSONAR - no <array> in tts
                                                       "N",
                                                       "Speedup",
                                                       "Eff.",
                                                       "ns/elem",
                                                       "min",
                                                       "max",
                                                       "err%",
                                                       "elem/s",
                                                       "cyc/elem",
                                                       "ins/elem",
                                                       "bra/elem",
                                                       "miss%"};
    static constexpr int         widths[ columns ]  = {// NOSONAR - no <array> in tts
                                                       4,
                                                       9,
                                                       8,
                                                       10,
                                                       10,
                                                       10,
                                                       6,
                                                       12,
                                                       10,
                                                       10,
                                                       10,
                                                       6};

    ankerl::nanobench::Bench     bench_;
    tts::buffer<std::size_t>     lanes_;
    tts::text                    title_;

    template<typename Row, typename Func, typename... Inputs> // NOSONAR - <iterator> is refused
    void time_run(tts::text const& name, Func& func, Inputs const&... in)
    {
      constexpr std::size_t N = Traits::template lanes<Row>;
      lanes_.push_back(N);

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

    static void dashes(tts::text& line, int count)
    {
      while(count-- > 0)
        line += "-";
    }

    static bool counted(ankerl::nanobench::Result const& res)
    {
      using measure = ankerl::nanobench::Result::Measure;
      return res.median(measure::cpucycles) > 0.0 || res.median(measure::instructions) > 0.0;
    }

    // One CSV column per row and one line per epoch. Do not divide by the lanes as well: an epoch
    // times one call of the lambda, a pass over the batch.
    void dump_csv() const
    {
      auto const&        results  = bench_.results();
      tts::text const    filename = title_ + ".csv";
      tts::_::file_guard file(std::fopen(filename.data(), "w"));
      if(!file)
      {
        tts::output().writeln("> Note: %s could not be opened for writing.", filename.data());
        return;
      }

      for(std::size_t r = 0; r < results.size(); ++r)
      {
        if(r != 0) std::fputc(',', file.get());
        _::quoted(file.get(), results[ r ].config().mBenchmarkName.c_str());
      }
      for(std::size_t e = 0; e < bench_.epochs(); ++e)
      {
        std::fputc('\n', file.get());
        for(std::size_t r = 0; r < results.size(); ++r)
        {
          if(r != 0) std::fputc(',', file.get());
          auto const&  res     = results[ r ];
          double const elapsed = res.get(e, ankerl::nanobench::Result::Measure::elapsed);
          std::fprintf(file.get(), "%g", elapsed / static_cast<double>(res.config().mBatch));
        }
      }
      std::fputc('\n', file.get());
    }

    static void
    print_row(std::size_t name_width, char const* name, tts::buffer<tts::text> const& cells)
    {
      tts::text line("| ");
      line += tts::text("%-*s", static_cast<int>(name_width), name);
      for(std::size_t c = 0; c < columns; ++c)
        line += tts::text(" | %*s", widths[ c ], cells[ c ].data());
      line += " |";
      tts::output().writeln(line);
    }

    tts::buffer<tts::text> row_cells(ankerl::nanobench::Result const& res,
                                     std::size_t                      i,
                                     double                           baseline_elem_per_sec) const
    {
      using measure             = ankerl::nanobench::Result::Measure;
      auto const   batch_size   = static_cast<double>(res.config().mBatch);
      std::size_t  N            = lanes_[ i ];
      double const current_time = res.median(measure::elapsed);
      double const elem_per_sec = batch_size / current_time;
      double const speedup      = elem_per_sec / baseline_elem_per_sec;
      double const efficiency   = speedup / static_cast<double>(N);
      double const branches     = res.median(measure::branchinstructions);
      bool const   has_counters = counted(res);

      return {tts::text("%zu", N),
              tts::text("x") + _::digits(speedup),
              (N == 1 && i != 0) ? tts::text("-") : tts::text("%.1f%%", efficiency * 100.0),
              _::digits(1e9 * current_time / batch_size),
              _::digits(1e9 * res.minimum(measure::elapsed) / batch_size),
              _::digits(1e9 * res.maximum(measure::elapsed) / batch_size),
              _::spread(res.medianAbsolutePercentError(measure::elapsed)),
              tts::text("%.0f", elem_per_sec),
              has_counters ? tts::text("%.2f", res.median(measure::cpucycles) / batch_size)
                           : tts::text("-"),
              has_counters ? tts::text("%.2f", res.median(measure::instructions) / batch_size)
                           : tts::text("-"),
              has_counters ? tts::text("%.2f", branches / batch_size) : tts::text("-"),
              branches > 0.0
              ? tts::text("%.1f%%", 100.0 * res.median(measure::branchmisses) / branches)
              : tts::text("-")};
    }

    void print_table() const
    {
      auto const& results    = bench_.results();
      auto&       out        = tts::output();
      std::size_t name_width = 4;
      for(auto const& res: results)
        name_width = tts::_::max(name_width, res.config().mBenchmarkName.size());

      tts::text header("| ");
      header += tts::text("%-*s", static_cast<int>(name_width), "Name");
      tts::text separator("|:");
      dashes(separator, static_cast<int>(name_width));
      separator += "-|";
      for(std::size_t c = 0; c < columns; ++c)
      {
        header += tts::text(" | %*s", widths[ c ], headers[ c ]);
        dashes(separator, widths[ c ] + 1);
        separator += ":|";
      }
      header += " |";
      out.writeln(header);
      out.writeln(separator);

      auto const& first    = results.front();
      auto const  baseline = static_cast<double>(first.config().mBatch) /
                             first.median(ankerl::nanobench::Result::Measure::elapsed);

      using measure        = ankerl::nanobench::Result::Measure;
      bool blocked         = false;
      bool unstable        = false;
      for(std::size_t i = 0; i < results.size(); ++i)
      {
        blocked  = blocked || !counted(results[ i ]);
        unstable = unstable || results[ i ].medianAbsolutePercentError(measure::elapsed) > 0.05;
        print_row(name_width,
                  results[ i ].config().mBenchmarkName.c_str(),
                  row_cells(results[ i ], i, baseline));
      }

      if(blocked || unstable) out.writeln();
      if(blocked)
        out.writeln("> Note: the OS blocks the hardware counters, so their four columns read -.");
      if(unstable)
        out.writeln(
        "> Note: ~ marks an err% above 5%, the threshold of nanobench for an unstable measure.");
    }

    void report() const
    {
      if(bench_.results().empty()) return;

      auto& out = tts::output();
      out.writeln("# Benchmarking for : `%s`", title_.data());
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
      if(::tts::arguments()("-d", "--dump")) dump_csv();
      out.writeln();

      print_table();

      out.writeln();
      out.flush();
    }

  public:
    //======================================================================================================================
    /**
      @brief Builds a benchmark whose table bears the title @p title.

      Both counts can be overridden with `--epochs=N` and `--warmup=N`.

      @param title  Title of the table, a string literal or a `tts::text`, also the base name of its
                    CSV file.
      @param epochs Number of epochs for each row, at least 1 (100 if not specified).
      @param warmup Number of warmup passes over all buffers (100 if not specified).
    **/
    //======================================================================================================================
    template<typename Name>
    explicit basic_benchmark(Name const& title, std::size_t epochs = 100, std::size_t warmup = 100)
        : title_(tts::text(title))
    {
      bench_.output(nullptr);
      // Do not read the counts as std::size_t: -1 would wrap to a count that no run ends.
      long long const e = ::tts::arguments().value(static_cast<long long>(epochs), "--epochs");
      long long const w = ::tts::arguments().value(static_cast<long long>(warmup), "--warmup");
      bench_.epochs(static_cast<std::size_t>(e < 1 ? 1 : e));
      bench_.warmup(static_cast<std::uint64_t>(w < 0 ? 0 : w));
    }

    basic_benchmark(basic_benchmark const&)            = delete;
    basic_benchmark& operator=(basic_benchmark const&) = delete;

    //======================================================================================================================
    /**
      @brief Prints the table, and writes the CSV file if `-d` or `--dump` is given.
    **/
    //======================================================================================================================
    ~basic_benchmark()
    {
      report();
    }

    //======================================================================================================================
    /**
      @brief Times a row of type `Row` (the function called by @ref TTS_RUN_BENCHMARK).

      @tparam Row     Type of the row, used by the traits to build its input and to count its
                      lanes.
      @param row_name Name of the row, as printed in the table.
      @param func     Function to time, called once per index of the buffers.
      @param values   Buffers holding the arguments for `func`, in parameter order.
    **/
    //======================================================================================================================
    template<typename Row, typename Name, typename Func, typename... Values>
    void run(Name const& row_name, Func&& func, Values const&... values)
    {
      time_run<Row>(tts::text(row_name), func, Traits::template input<Row>(values)...);
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
