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

#define TTS_RUN_BENCHMARK(RUNNER, TYPE, NAME, FUNC, ...)                                           \
  (RUNNER).run<TYPE>(NAME, FUNC, __VA_ARGS__) /**/

#define TTS_RUN_BENCHMARK_TPL(RUNNER, TYPE, NAME, FUNC, ...)                                       \
  (RUNNER).template run<TYPE>(NAME, FUNC, __VA_ARGS__) /**/

namespace tts::bench
{
  inline constexpr std::size_t size       = 1024;

  inline std::uint64_t         prng_state = 0x853c49e6748fea9bULL;

  // Do not draw from tts::_::prng_state as it stands: the run seeds it from the clock, and two
  // executions would time different values.
  template<typename T> T roll(T mini, T maxi)
  {
    std::uint64_t const run   = std::exchange(tts::_::prng_state, prng_state);
    T const             value = tts::_::roll(mini, maxi);
    prng_state                = std::exchange(tts::_::prng_state, run);
    return value;
  }

  template<typename T>
    requires(std::is_arithmetic_v<T>)
  tts::buffer<T> random(T mini, T maxi)
  {
    tts::buffer<T> values(size);
    for(auto& v: values)
      v = roll(mini, maxi);
    return values;
  }

  template<typename U, typename T> tts::buffer<U> convert(tts::buffer<T> const& values)
  {
    tts::buffer<U> out;
    for(auto const& v: values)
      out.push_back(U(v));
    return out;
  }

  struct default_traits
  {
    template<typename Row, typename Value>
    static tts::buffer<Value> const& input(tts::buffer<Value> const& values)
    {
      return values;
    }

    template<typename Row> static constexpr std::size_t lanes = 1;

    template<typename Result> static void               sink(Result const& r)
    {
      ankerl::nanobench::doNotOptimizeAway(r);
    }

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
      for(double m = 10.; decimals > 0 && v >= m; m *= 10.)
        --decimals;
      for(double m = 1.; v > 0. && v < m && decimals < 9; m /= 10.)
        ++decimals;
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

  template<typename Traits = default_traits> class basic_benchmark
  {
  private:
    ankerl::nanobench::Bench    bench_;
    std::size_t                 repetitions_;

    tts::buffer<std::ptrdiff_t> run_cardinals_;
    tts::text                   base_file_name_;

    void                        configure_bench()
    {
      bench_.output(nullptr);
      bench_.performanceCounters(true);
      bench_.epochs(repetitions_);
      bench_.warmup(1);
    }

    template<typename Row, typename Func, typename... Inputs>
    void time_run(tts::text const& name, Func& func, Inputs const&... in)
    {
      constexpr std::size_t N = Traits::template lanes<Row>;
      run_cardinals_.push_back(static_cast<std::ptrdiff_t>(N));

      std::size_t count = static_cast<std::size_t>(-1);
      ((count = tts::_::min(count, in.size())), ...);
      bench_.batch(count * N);

      bench_.run(name.data(),
                 [ & ]()
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

  public:
    explicit basic_benchmark(std::size_t repetitions = 100)
        : repetitions_(repetitions)
        , base_file_name_("benchmark")
    {
      configure_bench();
    }

    // Do not drop the constraint: an integer would select this constructor and be taken as a name.
    template<typename Name>
    explicit basic_benchmark(Name const& name, std::size_t repetitions = 100)
      requires(!std::is_integral_v<Name>)
        : repetitions_(repetitions)
        , base_file_name_(tts::text(name))
    {
      configure_bench();
    }

    basic_benchmark(basic_benchmark const&)            = delete;
    basic_benchmark& operator=(basic_benchmark const&) = delete;

    ~basic_benchmark()
    {
      report();
    }

    basic_benchmark& reset()
    {
      bench_         = ankerl::nanobench::Bench();
      run_cardinals_ = tts::buffer<std::ptrdiff_t>();
      configure_bench();
      return *this;
    }

    template<typename Row, typename Name, typename Func, typename... Values>
    basic_benchmark& run(Name const& name, Func&& func, Values const&... values)
    {
      execute_single_run<Row>(tts::text(name), func, values...);
      return *this;
    }

    basic_benchmark& report()
    {
      auto const& results = bench_.results();
      if(results.empty()) return *this;

      // One CSV column per run and one line per epoch, each time divided by the number of elements
      // of its run.
      tts::text dump_note;
      if(::tts::arguments()("-d", "--dump"))
      {
        tts::text          filename = base_file_name_ + ".csv";
        tts::_::file_guard out_file(std::fopen(filename.data(), "w"));

        if(out_file)
        {
          std::size_t epochs = 0;
          for(std::size_t r = 0; r < results.size(); ++r)
          {
            _::quoted(out_file.get(), results[ r ].config().mBenchmarkName.c_str());
            std::fputs(r + 1 < results.size() ? "," : "\n", out_file.get());
            epochs = tts::_::max(epochs, results[ r ].size());
          }

          for(std::size_t e = 0; e < epochs; ++e)
          {
            for(std::size_t r = 0; r < results.size(); ++r)
            {
              auto const& res = results[ r ];
              if(e < res.size())
              {
                // Do not divide by the cardinal of the run: an epoch times one call of the
                // lambda, a pass over the batch.
                double const elapsed = res.get(e, ankerl::nanobench::Result::Measure::elapsed);
                tts::text    val {"%g", elapsed / static_cast<double>(res.config().mBatch)};
                std::fputs(val.data(), out_file.get());
              }
              if(r + 1 < results.size()) std::fputs(",", out_file.get());
            }
            std::fputs("\n", out_file.get());
          }
        }
        else
        {
          dump_note = tts::text("> Note: %s could not be opened for writing.", filename.data());
        }
      }

      auto& out = tts::output();
      out.writeln("# Benchmarking for : `%s`", base_file_name_.data());
      if(tts::text const env = Traits::environment(); !env.is_empty()) out.writeln(env);
      if(!dump_note.is_empty()) out.writeln(dump_note);
      out.writeln();

      std::size_t name_width = 4;
      for(auto const& res: results)
      {
        name_width = tts::_::max(name_width, res.config().mBenchmarkName.size());
      }

      constexpr int columns            = 10;
      char const*   headers[ columns ] = {
      "N", "Speedup", "Eff.", "ns/elem", "min", "max", "err%", "elem/s", "cyc/elem", "ins/elem"};
      int const widths[ columns ] = {4, 9, 8, 10, 10, 10, 6, 12, 10, 10};

      auto      print_row         = [ & ](char const* name, tts::text const* cells)
      {
        tts::text line("| ");
        line += tts::text("%-*s", static_cast<int>(name_width), name);
        for(int c = 0; c < columns; ++c)
          line += tts::text(" | %*s", widths[ c ], cells[ c ].data());
        line += " |";
        out.writeln(line);
      };

      tts::text header_cells[ columns ];
      for(int c = 0; c < columns; ++c)
        header_cells[ c ] = tts::text(headers[ c ]);
      print_row("Name", header_cells);

      tts::text separator("|:");
      dashes(separator, static_cast<int>(name_width));
      separator += "-|";
      for(int c = 0; c < columns; ++c)
      {
        dashes(separator, widths[ c ] + 1);
        separator += ":|";
      }
      out.writeln(separator);

      using measure                = ankerl::nanobench::Result::Measure;

      double baseline_time         = results.front().median(measure::elapsed);
      double baseline_batch        = static_cast<double>(results.front().config().mBatch);
      double baseline_elem_per_sec = baseline_batch / baseline_time;

      bool   counters_failed       = false;

      for(std::size_t i = 0; i < results.size(); ++i)
      {
        auto const&    res          = results[ i ];
        double         current_time = res.median(measure::elapsed);
        double         batch_size   = static_cast<double>(res.config().mBatch);
        std::ptrdiff_t N            = run_cardinals_[ i ];

        double         elem_per_sec = batch_size / current_time;
        double         speedup      = elem_per_sec / baseline_elem_per_sec;
        double         efficiency   = speedup / static_cast<double>(N);

        double         ins_per_elem = res.median(measure::instructions) / batch_size;
        double         cyc_per_elem = res.median(measure::cpucycles) / batch_size;
        double         ns_per_elem  = 1e9 * current_time / batch_size;
        double         min_ns_elem  = 1e9 * res.minimum(measure::elapsed) / batch_size;
        double         max_ns_elem  = 1e9 * res.maximum(measure::elapsed) / batch_size;
        double         error        = res.medianAbsolutePercentError(measure::elapsed);

        if(cyc_per_elem == 0.0 && ins_per_elem == 0.0) counters_failed = true;

        tts::text cells[ columns ] = {tts::text("%td", N),
                                      tts::text("x") + _::digits(speedup),
                                      (N == 1 && i != 0) ? tts::text("-")
                                                         : tts::text("%.1f%%", efficiency * 100.0),
                                      _::digits(ns_per_elem),
                                      _::digits(min_ns_elem),
                                      _::digits(max_ns_elem),
                                      tts::text("%.1f%%", error * 100.0),
                                      tts::text("%.0f", elem_per_sec),
                                      tts::text("%.2f", cyc_per_elem),
                                      tts::text("%.2f", ins_per_elem)};

        print_row(res.config().mBenchmarkName.c_str(), cells);
      }

      if(counters_failed)
      {
        out.writeln();
        out.writeln("> Note: CPU cycles and instructions were 0.00. OS blocked hardware counters.");
      }

      out.writeln();
      out.flush();
      return *this;
    }
  };

  TTS_DISABLE_WARNING_POP

  using benchmark = basic_benchmark<default_traits>;
}
