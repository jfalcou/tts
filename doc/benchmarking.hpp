#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  benchmarking  First Benchmark

  @tableofcontents

  With **TTS**, functions can be benchmarked as well as tested. A benchmark is given the functions
  to compare and the data to run them on, and prints how long each function takes.
  [nanobench](https://nanobench.ankerl.com) takes the measurements. A complete benchmark fits in a
  single test case. Here, `std::hypot` is compared with the naive formula
  `std::sqrt(x * x + y * y)`:

  @snippet benchmarks/hypot.cpp snippet

  @section benchmarking-inputs Input data

  The functions run on values generated once, before anything is measured. That way, the time
  spent generating them stays out of the measurements, and every function runs on the same data.
  `tts::bench::random<T>(min, max)` returns a buffer of random values of type `T`, uniformly
  distributed between `min` and `max`. A buffer holds 1024 values unless a size is passed as a
  third argument. Since `std::hypot` takes two arguments, the benchmark uses two buffers:

  @code{.cpp}
  auto x = tts::bench::random<double>(-100., 100.);
  auto y = tts::bench::random<double>(-100., 100.);
  @endcode

  @section benchmarking-functions Functions

  Each function is called once per element and takes one parameter per buffer. The first call
  receives `x[0]` and `y[0]`, the second `x[1]` and `y[1]`, and so on up to the last element.
  Write the functions as lambdas (functions defined where they are used) or as regular
  [Callable objects](https://en.cppreference.com/w/cpp/named_req/Callable). The `double`
  parameters of a lambda select the `double` overloads of `std::hypot` and `std::sqrt`, and the
  compiler can inline a lambda in the measured loop, as it does in regular code:

  @code{.cpp}
  auto hypot = [](double u, double v) { return std::hypot(u, v); };
  auto naive = [](double u, double v) { return std::sqrt(u * u + v * v); };
  @endcode

  @section benchmarking-benchmark Benchmark

  A tts::bench::benchmark collects the results in a table whose title is passed when the benchmark
  is constructed. Each @ref TTS_RUN_BENCHMARK then measures a function and adds its row to the
  table. The macro takes the benchmark, the type of the values in the buffers, the name of the
  row, the function, and the buffers. The first row is the baseline, and every other row is
  compared with it:

  @code{.cpp}
  tts::bench::benchmark b("hypot");
  TTS_RUN_BENCHMARK(b, double, "std::hypot", hypot, x, y);
  TTS_RUN_BENCHMARK(b, double, "naive", naive, x, y);
  @endcode

  @section benchmarking-case Test case

  Write a benchmark inside a regular test case (see @ref tts101), with @ref TTS_MAIN and
  @ref TTS_CASE as in a test file. A benchmark file differs from a test file in two ways:

  - Include `tts/benchmarks.hpp` instead of `tts/tts.hpp`. This header brings in `tts/tts.hpp`
    and the benchmark utilities, which are kept out of `tts/tts.hpp` because they use nanobench.
    Include it in the file that defines @ref TTS_MAIN, since nanobench is compiled in that file.
  - End the case with a check. Measuring a function checks nothing, and **TTS** reports a test
    case without a check as invalid. Check that the functions agree on the input data, with
    @ref TTS_ULP_EQUAL for instance, or check the tts::bench::measurement that each
    @ref TTS_RUN_BENCHMARK returns, whose times are in microseconds. Without such a check,
    @ref TTS_PASS is enough.

  @section benchmarking-build Build

  A benchmark needs two include paths, one for **TTS** and one for `nanobench.h`, and no library
  to link. With CMake, turn on the `TTS_BENCHMARKS` option where the project adds **TTS**, for
  instance in the `OPTIONS` of `CPMAddPackage` (see @ref setup-benchmarks). **TTS** then fetches
  nanobench and defines the `tts::benchmarks` target. Link the executable to it to get both
  include paths:

  @code{cmake}
  add_executable(my_bench my_bench.cpp)
  target_link_libraries(my_bench PRIVATE tts::benchmarks)
  @endcode

  @warning Build a benchmark with the release flags of the project, that is, with optimizations
  on and assertions off, for instance `-O3 -DNDEBUG`. Otherwise the benchmark measures code that
  differs from the code that ships. The output shows whether assertions are enabled, but not
  whether optimizations are on.

  @section benchmarking-output Output

  Run the executable like any test. The benchmark prints its table when it goes out of scope,
  here at the end of the test case, so the table appears in the middle of the test report:

  @code{.unparsed}
  # Benchmarking for : `hypot`
  Assertions: disabled - Epochs: 101 - Warmup: 100

  | Name       |    ns/elem |        min |        max |   err% |    N |   Speedup |     Eff. |       elem/s |
  |:-----------|-----------:|-----------:|-----------:|-------:|-----:|----------:|---------:|-------------:|
  | std::hypot |       2.94 |       2.78 |       3.66 |   2.6% |    1 |     x1.00 |   100.0% |    340477175 |
  | naive      |       1.21 |       1.16 |       1.59 |   1.9% |    1 |     x2.42 |        - |    824285206 |

  > Note: the OS blocks the performance counters so none are displayed.
  @endcode

  The numbers depend on the machine and on the compiler. In this run:

  - The `Assertions` line, under the title, shows the settings of the run. Assertions are
    disabled. Each function first runs 100 warmup passes over the buffers, then is measured 101
    times, once per epoch. Both counts are the defaults.
  - `ns/elem` is the time per element in a loop over the buffers, in nanoseconds. A call processes
    one element here, so it is also the time per call: 2.94 ns for `std::hypot` and 1.21 ns for the
    naive formula. It is the median of the epochs, while `min` and `max` are the fastest and the
    slowest of them.
  - `err%` measures how much the epochs fluctuate around their median. Both rows stay under 5%,
    the threshold at which a `~` flags a measurement as unstable.
  - `Speedup` compares each row with the baseline. The naive formula is 2.42 times faster than
    `std::hypot`, which does more work to avoid overflow and underflow in the squares.
  - `N` is the number of elements a call processes, 1 here. `Eff.` is the speedup divided by `N`,
    in percent. With an `N` of 1, it reads 100% on the baseline and `-` on the other rows, where
    it would repeat the speedup.
  - `elem/s` is the number of elements per second, the inverse of the median time.
  - The CPU performance counters give four more columns, `cyc/elem`, `ins/elem`, `bra/elem`, and
    `miss%`. The system of this run blocks these counters, as containers and virtual machines
    often do, so the table leaves them out and a note follows it.

  <div class="section_buttons">

  | Previous |                                              Next |
  |:---------|--------------------------------------------------:|
  |          | [Designing a Benchmark](@ref benchmarking-design) |

  </div>
**/
//==================================================================================================
