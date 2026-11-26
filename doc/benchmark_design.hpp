#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  benchmarking-design  Designing a Benchmark

  @tableofcontents

  A benchmark runs a piece of code many times and reports statistics on the time it takes.
  A single call cannot be measured reliably. A call to a small function takes a few nanoseconds,
  less than it takes to read the clock. Its time also changes with its arguments, with the code
  that ran before it, and with whatever else the machine is doing.

  With **TTS**, the benchmarked code is a function, called on every element of the buffers that
  hold its input data. Such a benchmark fits functions that run in a loop over data, such as math
  functions and numerical kernels, scalar or SIMD. It does not fit code that reads a file, or
  whose calls depend on the calls before them, since every call has to be repeatable. A benchmark
  compares functions that do the same job, for instance a new implementation against the standard
  one.

  @section benchmarking-design-measurement Measurement

  nanobench measures each function in two steps:

  - First comes the warmup, which runs the function on the buffers without measuring anything.
    It brings the data into the cache and trains the branch predictor.
  - Then come the epochs, the measurements themselves (epoch is the nanobench term). In each
    epoch, the function runs through all the buffers several times back to back. nanobench sets the
    number of passes so that the clock can measure the total time accurately (see its
    [reference](https://nanobench.ankerl.com/reference.html)). That time is then divided by the
    number of calls.

  The `ns/elem` of a function is the median of its epochs. It is the inverse of a throughput, the
  time per call in a loop over data already in the cache, and not the latency of a single call on
  cold data. The result of every call is passed to `ankerl::nanobench::doNotOptimizeAway`. Without
  it, the compiler could optimize the call away, since nothing would use the result.

  By default, each function gets 100 warmup passes through its buffers, then 101 epochs.
  With fewer epochs, a slow function takes less time to benchmark, but its median is noisier.
  Pass both counts as the second and third arguments of tts::bench::benchmark. On the command
  line, `--epochs=N` and `--warmup=N` override them for every benchmark in the executable
  (see @ref cli). A count below its minimum, 1 epoch or 0 warmup passes, is raised to that
  minimum. An even number of epochs is raised to the next odd one, so that the median of a
  function is the time of one epoch. Here, `std::cbrt` gets 10 warmup passes, then 21 epochs:

  @snippet benchmarks/counts.cpp snippet

  @section benchmarking-design-inputs Input data

  The time of a function depends on its arguments. Many mathematical functions are slower on huge
  or tiny values than on ordinary ones, and a mispredicted branch takes longer than a predicted
  one. Run all the functions on the same values, chosen like the ones they receive in real use.

  - tts::bench::random returns a `tts::buffer` (the **TTS** vector type) of values uniformly
    distributed between two bounds, both included. Its third argument is the number of values, and
    defaults to tts::bench::size, which is 1024.
  - tts::bench::roll returns one such value. A buffer of a type that tts::bench::random does not
    handle is filled with it element by element. Both functions work for every arithmetic type,
    `bool` included.
  - A `tts::buffer` can be created empty or with a number of elements. It has `size()`,
    `operator[]`, and `push_back`, and works in a range-based for loop.
  - @ref TTS_RUN_BENCHMARK also accepts any other container with `size()` and `operator[]`, such
    as a `std::vector`.

  The random values come from the **TTS** random number generator. Its seed is printed when the
  executable starts. Pass that seed back with `--seed` to run the benchmark again on the same
  values. All the test cases of an executable use the same generator, in the order they run. The
  input data of a case depends on the cases that ran before it, and changes when `--shard` skips
  some of them. Here, a buffer of intervals is filled from two random values per element, and a
  buffer of 64 flags is generated:

  @snippet benchmarks/inputs.cpp snippet

  @section benchmarking-design-functions Functions

  @ref TTS_RUN_BENCHMARK measures its function when execution reaches the macro, and the rows
  appear in the order of the macros. The function is called once per element, and the shortest
  buffer sets the number of calls. A function that takes its parameters by `const&` reads the
  values without a copy.

  The function can be a lambda, a
  [callable object](https://en.cppreference.com/w/cpp/named_req/Callable), or the name of a
  function. The compiler can inline a lambda or a callable object in the measured loop, as it does
  in regular code. A function name is passed as a pointer, while the name of an overloaded function
  such as `std::exp` cannot be passed, since it names several functions. Call such a function from a
  lambda. Here, a lambda and a function name compute the same division, and `std::exp` is called
  from a lambda:

  @snippet benchmarks/calls.cpp snippet

  @warning The compiler may keep calls through a function pointer indirect, which slows the
  function down and changes its speedup. In a run of this example, `halve` took 1.00 ns per call
  when passed by name, and the lambda that does the same division took 0.406 ns. Inside a lambda,
  the call to the function is direct, and the compiler can inline it.

  @section benchmarking-design-baseline Baseline

  Every table has a baseline, its first row, against which the other rows are compared. The
  `Speedup` of a row is the baseline time divided by the time of the row. Make the first row the
  function that the others have to beat, the standard function or the implementation to replace.
  Functions that need another baseline go in another tts::bench::benchmark, which prints a table
  of its own.

  A benchmark prints its table from its destructor, so two benchmarks in the same scope print in
  the reverse order of their construction. Put each benchmark in a block of its own to print the
  tables in the order of the code.

  @section benchmarking-design-types Types

  A function is often benchmarked for several types. A template test case, written with
  @ref TTS_CASE_TPL, runs once per type and builds one benchmark per type. Each type gets its own
  table and its own baseline. Put the name of the type in the title with tts::as_text and
  tts::typename_. A buffer generated inside the case holds new values for each type. To run two
  types on the same values, generate one buffer once, outside the case, and give each type a copy
  converted with tts::bench::convert. Here, one buffer generated in `float` is
  used for both `float` and `double`:

  @snippet benchmarks/types.cpp snippet

  <div class="section_buttons">

  | Previous                             |                                             Next |
  |:-------------------------------------|-------------------------------------------------:|
  | [First Benchmark](@ref benchmarking) | [Reading the Results](@ref benchmarking-results) |

  </div>
**/
//==================================================================================================
