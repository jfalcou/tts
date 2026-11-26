//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tts.hpp>

#if defined(TTS_MAIN) && !defined(ANKERL_NANOBENCH_IMPLEMENT)
#define ANKERL_NANOBENCH_IMPLEMENT
#endif
#include <nanobench.h>

#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>
#include <vector>

//======================================================================================================================
/**
  @defgroup tools-benchmarks Benchmark Utilities
  @brief Tools for benchmarking functions with [nanobench](https://nanobench.ankerl.com) and
  printing their time per call and throughput as Markdown tables.

  `tts/benchmarks.hpp` provides tts::bench::basic_benchmark, which measures functions as the rows of
  a table and prints that table, @ref TTS_RUN_BENCHMARK, which measures one function and returns
  its tts::bench::measurement, and the functions that generate the input data of the rows (see
  @ref benchmarking "Benchmarking with TTS").

  The header is separate from `tts/tts.hpp`, which it includes, and it includes `nanobench.h` (see
  @ref setup-benchmarks). The standalone branch holds it as `benchmarks.hpp`, a single file that
  contains all of **TTS** (see @ref setup-standalone). The translation unit that defines
  @ref TTS_MAIN has to include this header, so that the implementation of nanobench is compiled in
  that unit, with no other macro to define:
  @code
  #define TTS_MAIN
  #include <tts/benchmarks.hpp>
  @endcode

  @groupheader{Example}
  @snippet benchmarks/hypot.cpp snippet
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_RUN_BENCHMARK
  @brief Measures a function as a row of a benchmark.

  The function is measured when the macro runs. The traits of the benchmark build its inputs
  first, outside the measured time. Each pass over the buffers calls the function once per index,
  up to the size of the shortest buffer, and passes the result of each call to the
  `prevent_optimization` member of the traits.
  A lambda or a [callable object](https://en.cppreference.com/w/cpp/named_req/Callable) can be
  inlined in the measured loop, while a function name passes a pointer, whose calls the compiler
  may leave indirect.

  The macro returns the tts::bench::measurement of the row, whose fields a case can check. Running
  a benchmark adds no check to the case, so **TTS** reports a case that only runs benchmarks as
  invalid (see @ref TTS_CASE).

  The macro also serves a benchmark whose type depends on a template parameter, such as one built
  in a @ref TTS_CASE_TPL, since it writes the `.template` keyword.

  @param RUNNER Benchmark receiving the row.
  @param TYPE   Type of the row, a label that the traits read to build its input and to count its
                lanes. Under `tts::bench::default_traits`, any type serves, such as the type of
                the values, and `N` is 1.
  @param NAME   Name of the row, as printed in the table.
  @param FUNC   Function to measure, called once per index of the buffers. A function taking its
                parameters by `const&` reads them without a copy.
  @param ...    Buffers from which the traits build the arguments of `FUNC`, in parameter order.
                Under `tts::bench::default_traits`, the `i`-th call takes the `i`-th value of each
                buffer.

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
    @brief The default number of values in each buffer that `tts::bench::random` generates (a
           multiple of the lane counts of common SIMD registers).

    @groupheader{Example}
    @snippet benchmarks/inputs.cpp snippet
  **/
  //======================================================================================================================
  inline constexpr std::size_t size = 1024;

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Generates a value uniformly distributed over [mini, maxi] with the random generator of
           the run, as `tts::bench::random` does for each element.

    @tparam T   Arithmetic type of the value, `bool` included.
    @param mini Inclusive lower bound of the value.
    @param maxi Inclusive upper bound of the value.
    @return A value of type `T` between `mini` and `maxi`.

    @groupheader{Example}
    @snippet benchmarks/inputs.cpp snippet
  **/
  //======================================================================================================================
  template<typename T>
    requires(std::is_arithmetic_v<T>)
  T roll(T mini, T maxi)
  {
    return tts::_::roll(mini, maxi);
  }

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Generates a buffer of `count` random values uniformly distributed over [mini, maxi].

    Floating-point values are uniformly distributed, whereas those of `tts::random_value` are
    log-uniformly distributed. A floating-point value is the interpolation of the bounds at a
    uniform fraction, rounded to `T`. Each representable value has a probability proportional to
    the width of its rounding cell, which differs visibly on a range of few representable values.
    Each value of a `bool` buffer generated on [false, true] is `true` or `false` with equal odds.

    The values come from the random generator of the run, so `--seed` with the seed printed at
    startup repeats the values of a run. Each case of an executable, and each type of a
    @ref TTS_CASE_TPL, uses this generator in turn, so the values of a case depend on the cases
    that ran before it, under `--shard` as well.

    @tparam T    Arithmetic type of the values, `bool` included.
    @param mini  Inclusive lower bound of the values.
    @param maxi  Inclusive upper bound of the values.
    @param count Number of values to generate (`tts::bench::size` if not specified).
    @return A `tts::buffer`, the vector type of **TTS**, holding `count` values of type `T`.

    @groupheader{Example}
    @snippet benchmarks/inputs.cpp snippet
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
    @brief Converts every value of a buffer to `U`, so that a row can run on the same values in
           another type.

    @tparam U     Type to which every value is converted.
    @param values Buffer to convert.
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
    @brief Traits under which a function runs on its buffers unchanged and each call processes a
           single element.

    `tts::bench::basic_benchmark` uses four members of its traits, and builds the input of a row
    once, before its measurement.

    - `input<Row>(values)` builds the input of a row from a buffer, by value or by reference,
      and that input lives until the row has been measured.
    - `lanes<Row>` is the number of values that a call processes.
    - `prevent_optimization(result)` keeps a result from being optimized away.
    - `environment()` returns a line printed under the line of the assertions and the counts.

    A traits class derived from `default_traits` replaces only the members it needs. As a variable
    template, `lanes` can give each type of row its own count, and the table divides the time of a
    row by that count, whatever its function computes. Here, `lanes` gives four values per call to
    every row:
    @code
    struct four_lanes : tts::bench::default_traits
    {
      template<typename Row> static constexpr std::size_t lanes = 4;
    };
    @endcode

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
      @brief The number of values that each call processes for a row of type `Row`.
    **/
    //======================================================================================================================
    template<typename Row> static constexpr std::size_t lanes = 1;

    //======================================================================================================================
    /**
      @brief Keeps a result from being optimized away, through
             [`ankerl::nanobench::doNotOptimizeAway`](https://nanobench.ankerl.com/reference.html).
    **/
    //======================================================================================================================
    template<typename Result> static void prevent_optimization(Result const& result)
    {
      ankerl::nanobench::doNotOptimizeAway(result); // NOSONAR - the parameter shadows no name
    }

    //======================================================================================================================
    /**
      @brief Returns an empty text, which the benchmark does not print.
    **/
    //======================================================================================================================
    static tts::text environment()
    {
      return tts::text {};
    }
  };

  //======================================================================================================================
  /**
    @public
    @ingroup tools-benchmarks
    @brief Measurement of a row, as @ref TTS_RUN_BENCHMARK returns it.

    Its times are in microseconds per element, at full precision. The table prints the same times
    in nanoseconds, rounded. A row of `N` lanes has its times divided by `N`, as in the table. The
    speedup of a row B over a row A is the `time` of A divided by the `time` of B. The four fields
    come in the order `time`, `fastest`, `slowest`, and `error`, which a structured binding
    follows.

    @groupheader{Example}
    @snippet benchmarks/measurement.cpp snippet
  **/
  //======================================================================================================================
  struct measurement
  {
    /**
      @brief Median time per element over the epochs, in microseconds (`ns/elem` in the table).

      The number of epochs is odd, so the median is the time of one epoch.
    **/
    double time;

    /**
      @brief Time per element of the fastest epoch, in microseconds (`min` in the table).
    **/
    double fastest;

    /**
      @brief Time per element of the slowest epoch, in microseconds (`max` in the table).
    **/
    double slowest;

    /**
      @brief Median of \f$|t - m| / t\f$ over the epochs, `err%` divided by 100.

      \f$t\f$ is the time per element of an epoch and \f$m\f$ the median of these times. A row is
      unstable from 0.05, and a single epoch gives 0.
    **/
    double error;
  };

  namespace _
  {
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

    inline tts::text spread(double error)
    {
      tts::text cell("%.1f%%", error * 100.0);
      if(error >= 0.05) cell += "~";
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
    @brief Measures functions with [nanobench](https://nanobench.ankerl.com) as the rows of a
           table, and prints this table when the benchmark goes out of scope.

    The first row is the baseline of every speedup. An epoch is a sequence of complete passes over
    all the buffers of a row, and nanobench calibrates the passes per epoch of each row to reach its
    target time (left at the [defaults of nanobench](https://nanobench.ankerl.com/reference.html)).

    @groupheader{Title lines}

    The title line names the benchmark. Under it, the line `Assertions: disabled - Epochs: 101 -
    Warmup: 100` gives the state of the assertions in the translation unit (from `NDEBUG`) and both
    counts. Assertions left enabled in the measured functions run inside the measured calls, so a
    benchmark built without `NDEBUG` measures them too, and `-DNDEBUG` on the compile line removes
    them.

    If `environment()` of the traits returns a line that is not empty, the benchmark prints that
    line next.

    @groupheader{Columns}

    Each call of a single-lane row processes one element, whatever the type of its arguments, and
    each call of a row with `N` lanes (the values that a SIMD function processes at once) processes
    `N` elements. The table gives each @ref TTS_RUN_BENCHMARK its row, with these columns:

    | Column | Content |
    |---|---|
    | `ns/elem`, `min`, `max` | median, smallest, and largest time per element, in nanoseconds |
    | `err%` | median absolute percentage error of the epoch times, followed by `~` from 5% |
    | `N` | values processed per call, from the traits |
    | `Speedup` | median time per element of the first row over that of the row |
    | `Eff.` | the speedup over `N` in percent, `-` on a single-lane row after the first |
    | `elem/s` | elements per second, the inverse of the median time per element |
    | `cyc/elem` | median CPU cycles per element |
    | `ins/elem` | median instructions per element |
    | `bra/elem` | median branch instructions per element |
    | `miss%` | median mispredicted branches over median branch instructions, in percent |

    @groupheader{Notes}

    `err%` is the median of `|t - m| / t` over the epochs (`t` being the time per element in an
    epoch, and `m` their median), as nanobench defines it.

    `err%` is the spread of the epochs, which is not the uncertainty of their median. From 5%,
    nanobench flags a measurement as unstable, and the `~` follows the unrounded value, so a printed
    `5.0%` can lack it. With few epochs, `err%` means little, and a single epoch gives 0%.

    If the operating system blocks the CPU performance counters (as many containers do), the table
    leaves out their four columns, and a note follows the table, as for an `err%` of 5% or more.

    @groupheader{CSV file}

    With `-d` or `--dump`, the executable also writes `<title>.csv` to the working directory,
    replacing any earlier file of that title. The file holds:

    - the header line of quoted row names.
    - one line per epoch, holding the time per element of every row, in seconds.

    Rows are measured one after the other, so each line puts side by side epochs taken at
    different times. The file of a benchmark titled `hypot`, with two rows, starts as follows:
    @code{.unparsed}
    "std::hypot","naive"
    3.26695e-09,1.33364e-09
    3.09817e-09,1.23072e-09
    @endcode

    If this file cannot be opened (for instance, because the title contains a `/`), a note above the
    table reports the failure.

    @tparam Traits Class providing `input`, `lanes`, `prevent_optimization`, and `environment`, as
                   `tts::bench::default_traits` does.

    @groupheader{Example}
    @snippet benchmarks/traits.cpp snippet
  **/
  //======================================================================================================================
  template<typename Traits> class basic_benchmark
  {
  private:
    struct column
    {
      char const* header;
      int         width;
    };

    inline static std::vector<column> const columns = {
    { "ns/elem", 10},
    {     "min", 10},
    {     "max", 10},
    {    "err%",  6},
    {       "N",  4},
    { "Speedup",  9},
    {    "Eff.",  8},
    {  "elem/s", 12},
    {"cyc/elem", 10},
    {"ins/elem", 10},
    {"bra/elem", 10},
    {   "miss%",  6}
    };

    static constexpr std::size_t counter_columns = 4;

    ankerl::nanobench::Bench     bench_;
    tts::buffer<std::size_t>     lanes_;
    tts::text                    title_;

    template<typename Row, typename Func, typename... Inputs>
    measurement time_run(tts::text const& name, Func& func, Inputs const&... in)
    {
      constexpr std::size_t N = Traits::template lanes<Row>;
      lanes_.push_back(N);

      auto count = static_cast<std::size_t>(-1);
      ((count = tts::_::min(count, std::size(in))), ...);
      bench_.batch(count * N);

      bench_.run(name.data(),
                 [ &count, &func, &in... ]()
                 {
                   for(std::size_t i = 0; i < count; ++i)
                   {
                     decltype(auto) res = func(in[ i ]...);
                     Traits::prevent_optimization(res);
                   }
                 });
      return measured(bench_.results().back());
    }

    static bool counted(ankerl::nanobench::Result const& res)
    {
      using measure = ankerl::nanobench::Result::Measure;
      return res.median(measure::cpucycles) > 0.0 || res.median(measure::instructions) > 0.0;
    }

    static measurement measured(ankerl::nanobench::Result const& res)
    {
      using measure       = ankerl::nanobench::Result::Measure;
      auto const   batch  = static_cast<double>(res.config().mBatch);
      double const median = res.median(measure::elapsed);
      return {1e6 * median / batch,
              1e6 * res.minimum(measure::elapsed) / batch,
              1e6 * res.maximum(measure::elapsed) / batch,
              res.medianAbsolutePercentError(measure::elapsed)};
    }

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
          double const element = elapsed / static_cast<double>(res.config().mBatch);
          std::fprintf(file.get(), "%g", element); // NOSONAR - C++23 on a C++20 project
        }
      }
      std::fputc('\n', file.get());
    }

    static void print_row(std::size_t                   name_width,
                          std::size_t                   shown,
                          char const*                   name,
                          tts::buffer<tts::text> const& cells)
    {
      tts::text line("| ");
      line += tts::text("%-*s", static_cast<int>(name_width), name);
      for(std::size_t c = 0; c < shown; ++c)
        line += tts::text(" | %*s", columns[ c ].width, cells[ c ].data());
      line += " |";
      tts::output().writeln(line);
    }

    tts::buffer<tts::text>
    row_cells(ankerl::nanobench::Result const& res, std::size_t i, double baseline_time) const
    {
      using measure                  = ankerl::nanobench::Result::Measure;
      auto const        batch_size   = static_cast<double>(res.config().mBatch);
      std::size_t       N            = lanes_[ i ];
      measurement const m            = measured(res);
      double const      speedup      = baseline_time / m.time;
      double const      efficiency   = speedup / static_cast<double>(N);
      double const      branches     = res.median(measure::branchinstructions);
      bool const        has_counters = counted(res);

      return {_::digits(1e3 * m.time),
              _::digits(1e3 * m.fastest),
              _::digits(1e3 * m.slowest),
              _::spread(m.error),
              tts::text("%zu", N),
              tts::text("x") + _::digits(speedup),
              (N == 1 && i != 0) ? tts::text("-") : tts::text("%.1f%%", efficiency * 100.0),
              tts::text("%.0f", 1e6 / m.time),
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

      std::size_t counted_rows = 0;
      for(auto const& res: results)
        if(counted(res)) ++counted_rows;
      std::size_t const shown =
      counted_rows == 0 ? columns.size() - counter_columns : columns.size();

      tts::text header("| ");
      header += tts::text("%-*s", static_cast<int>(name_width), "Name");
      tts::text separator("|:");
      separator += std::string(name_width, '-').c_str();
      separator += "-|";
      for(std::size_t c = 0; c < shown; ++c)
      {
        header    += tts::text(" | %*s", columns[ c ].width, columns[ c ].header);
        separator += std::string(static_cast<std::size_t>(columns[ c ].width) + 1, '-').c_str();
        separator += ":|";
      }
      header += " |";
      out.writeln(header);
      out.writeln(separator);

      double const baseline = measured(results.front()).time;

      using measure         = ankerl::nanobench::Result::Measure;
      bool const blocked    = counted_rows < results.size();
      bool       unstable   = false;
      for(std::size_t i = 0; i < results.size(); ++i)
      {
        unstable = unstable || results[ i ].medianAbsolutePercentError(measure::elapsed) >= 0.05;
        print_row(name_width,
                  shown,
                  results[ i ].config().mBenchmarkName.c_str(),
                  row_cells(results[ i ], i, baseline));
      }

      if(blocked || unstable) out.writeln();
      if(counted_rows == 0)
        out.writeln("> Note: the OS blocks the performance counters so none are displayed.");
      else if(blocked)
        out.writeln("> Note: the OS blocks the performance counters of some rows, so their four "
                    "columns read -.");
      if(unstable)
        out.writeln("> Note: ~ marks an err% of 5% or more, the threshold of nanobench for an "
                    "unstable measurement.");
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

      Both counts can be overridden with `--epochs=N` and `--warmup=N` (see @ref cli), and a count
      below its minimum, 1 epoch and 0 warmup passes, is raised to that minimum. An even number of
      epochs is raised to the next odd one, so that the median of a row is the time of one epoch.

      @param title  Title of the table, a string literal or a `tts::text`, also the base name of its
                    CSV file.
      @param epochs Number of epochs for each row, odd and at least 1 (101 if not specified).
      @param warmup Number of warmup passes over all buffers (100 if not specified).
    **/
    //======================================================================================================================
    template<typename Name>
    explicit basic_benchmark(Name const& title, std::size_t epochs = 101, std::size_t warmup = 100)
        : title_(tts::text(title))
    {
      bench_.output(nullptr);
      long long const e = ::tts::arguments().value(static_cast<long long>(epochs), "--epochs");
      long long const w = ::tts::arguments().value(static_cast<long long>(warmup), "--warmup");
      auto const      n = static_cast<std::size_t>(e < 1 ? 1 : e);
      bench_.epochs(n % 2 == 0 ? n + 1 : n);
      bench_.warmup(static_cast<std::uint64_t>(w < 0 ? 0 : w));
    }

    basic_benchmark(basic_benchmark const&)            = delete;
    basic_benchmark& operator=(basic_benchmark const&) = delete;

    //======================================================================================================================
    /**
      @brief Prints the table, and writes the CSV file if `-d` or `--dump` is given, when at least
             one row was measured.

      Two benchmarks of the same scope thus print in the reverse order of their construction.
    **/
    //======================================================================================================================
    ~basic_benchmark()
    {
      report();
    }

    //======================================================================================================================
    /**
      @brief Measures a function as a row of type `Row` (the function called by
             @ref TTS_RUN_BENCHMARK).

      @tparam Row     Type of the row, used by the traits to build its input and to count its
                      lanes.
      @param row_name Name of the row, as printed in the table.
      @param func     Function to measure, called once per index of the buffers.
      @param values   Buffers holding the arguments for `func`, in parameter order.
      @return The measurement of the row.
    **/
    //======================================================================================================================
    template<typename Row, typename Name, typename Func, typename... Values>
    measurement run(Name const& row_name, Func func, Values const&... values)
    {
      return time_run<Row>(tts::text(row_name), func, Traits::template input<Row>(values)...);
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
