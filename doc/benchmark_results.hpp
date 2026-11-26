#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  benchmarking-results  Reading the Results

  @tableofcontents

  Each row of a benchmark table gives two main numbers, the time per element and the speedup over
  the baseline. The rest of the output shows whether assertions are enabled, how stable the
  measurements are, and what the CPU performance counters record.

  @section benchmarking-results-table Table

  A benchmark prints its title, then the `Assertions` line, which also gives the epoch and warmup
  counts, then one row per @ref TTS_RUN_BENCHMARK (see tts::bench::basic_benchmark). All times
  are per element. An element is one of the `N` values that a call processes.

  - `ns/elem` is the median time per element over the epochs of the row, in nanoseconds. `min` and
    `max` are the fastest and the slowest of these epochs. `elem/s` is the inverse of the median.
  - `err%` measures how much the epochs fluctuate around their median.
  - `Speedup` is the median time of the baseline divided by the median time of the row.
  - `N` is the number of elements that one call processes, and `Eff.` is the speedup divided by
    `N`, in percent. A baseline with an `N` of 1 reads 100%. The other rows with an `N` of 1 read
    `-`, since their `Eff.` would only repeat the speedup.
  - `cyc/elem`, `ins/elem`, `bra/elem`, and `miss%` come from the CPU performance counters: the
    cycles, the instructions, and the branches per element, and the share of mispredicted
    branches. When the system blocks these counters, as containers and virtual machines often do,
    the table leaves out these four columns, and a note follows it.

  Here is one run of the hypot benchmark with a third row, the overhead row, whose function only
  adds its two arguments. `std::hypot` takes 3.12 ns per call, the naive formula is 2.50 times
  faster, and the overhead function takes 0.596 ns:

  @code{.unparsed}
  # Benchmarking for : `hypot`
  Assertions: disabled - Epochs: 101 - Warmup: 100

  | Name       |    ns/elem |        min |        max |   err% |    N |   Speedup |     Eff. |       elem/s |
  |:-----------|-----------:|-----------:|-----------:|-------:|-----:|----------:|---------:|-------------:|
  | std::hypot |       3.12 |       3.03 |       5.27 |   1.8% |    1 |     x1.00 |   100.0% |    320662785 |
  | naive      |       1.25 |       1.18 |       3.25 |   1.1% |    1 |     x2.50 |        - |    801564182 |
  | overhead   |      0.596 |      0.568 |       1.05 |   1.0% |    1 |     x5.23 |        - |   1677524779 |

  > Note: the OS blocks the performance counters so none are displayed.
  @endcode

  @section benchmarking-results-assertions Assertions

  The `Assertions` line reads `enabled` when `NDEBUG` is not defined in the source file of the
  benchmark, and `disabled` when it is. Assertions left enabled in the measured functions run
  inside the measured calls, so a benchmark built without `NDEBUG` measures them too. Add
  `-DNDEBUG` to the compiler flags to remove them. With assertions enabled, the benchmark prints
  this line:

  @code{.unparsed}
  Assertions: enabled - Epochs: 101 - Warmup: 100
  @endcode

  @section benchmarking-results-stability Stability

  nanobench defines `err%` as the median of \f$|t - m| / t\f$ over the epochs of a row, where
  \f$t\f$ is the time per element of an epoch and \f$m\f$ the median of these times. It measures
  how much the epochs of one run fluctuate, which is different from the uncertainty of their
  median. At 5% or more, nanobench flags the measurement as unstable. A `~` follows the value, and
  a note follows the table. The threshold applies to the value before it is rounded, so a printed
  5.0% can appear without a `~`. With few epochs, `err%` is unreliable, and a single epoch gives
  0%.

  A row is often unstable because the machine is busy with other work, or because the row has too
  few epochs. Run the benchmark again on a quiet machine, or with more epochs, before reading
  anything into its speedup. Here, `std::cbrt` is measured over 5 epochs without warmup:

  @code{.unparsed}
  # Benchmarking for : `cbrt`
  Assertions: disabled - Epochs: 5 - Warmup: 0

  | Name      |    ns/elem |        min |        max |   err% |    N |   Speedup |     Eff. |       elem/s |
  |:----------|-----------:|-----------:|-----------:|-------:|-----:|----------:|---------:|-------------:|
  | std::cbrt |       9.75 |       7.27 |       11.7 | 16.4%~ |    1 |     x1.00 |   100.0% |    102534632 |

  > Note: the OS blocks the performance counters so none are displayed.
  > Note: ~ marks an err% of 5% or more, the threshold of nanobench for an unstable measurement.
  @endcode

  @section benchmarking-results-speedup Speedup

  A speedup is the ratio of two medians, and the table gives no confidence interval for it. The
  epochs of one run do not capture the variation from one run to the next, and successive epochs
  are not independent. To see how much a speedup varies, run the same executable several times.
  Runs with the same `--seed` use the same input data. Runs with different seeds also differ in
  their input data. Without `--seed`, **TTS** takes the seed from the clock, so runs started
  within the same second get the same seed.

  Over \f$n\f$ independent runs, the smallest and the largest speedup of a row form a confidence
  interval for its median speedup over all possible runs, with a confidence level of at least
  \f$1 - 2 / 2^n\f$, about 94% for five runs. When all \f$n\f$ speedups of a row exceed 1, its
  median speedup is above 1 with a confidence of at least \f$1 - 1 / 2^n\f$, about 97% for five
  runs. Between two rows A and B other than the baseline, the speedup of B over A is the `ns/elem`
  of A divided by the `ns/elem` of B. Here, five runs use five sets of input data:

  @code{.sh}
  for seed in 1 2 3 4 5
  do
    ./my_bench --seed=$seed
  done
  @endcode

  @warning These runs cover one executable on one machine. A different compiler, different flags
  such as `-march=native`, or a different processor give a different speedup, and more runs of
  this executable do not show it.

  @section benchmarking-results-overhead Overhead and speedup

  Every time in the table includes an overhead: the loop, the loads of the input values, and the
  code that keeps each result from being optimized away. So a speedup is not the ratio of the
  times that the two functions take on their own. An out-of-order processor runs part of the loop
  while the function computes. So the overhead in each row lies between zero and the time \f$o\f$
  of the overhead row, as long as the overhead function loads the same data as the others. For two
  rows with times \f$t_1\f$ and \f$t_2\f$, the ratio of the times that the two functions take on
  their own lies between \f$(t_1 - o) / t_2\f$ and \f$t_1 / (t_2 - o)\f$. For `std::hypot` and the
  naive formula in the table above, whose speedup is 2.50, these bounds are
  \f$(3.12 - 0.596) / 1.25\f$, about 2.02, and \f$3.12 / (1.25 - 0.596)\f$, about 4.77. The
  bounds vary from run to run, like the times they come from. They widen as the time of a row gets
  close to that of the overhead row, where the benchmark measures the loop more than the function.

  @section benchmarking-results-measurement Checks on measurements

  @ref TTS_RUN_BENCHMARK returns the tts::bench::measurement of its row, which holds the numbers of
  the table before rounding, so a test case can check them:

  - `time`, `fastest`, and `slowest` hold the `ns/elem`, `min`, and `max` columns in microseconds
    per element instead of nanoseconds, so a `time` of 0.003 appears as 3.00 in the table.
  - `error` is the `err%` column divided by 100, 0.05 for 5%.

  The speedup of a row B over a row A is the `time` of A divided by the `time` of B, which is the
  `Speedup` column of B when A is the baseline. Like that column, it includes the overhead (see
  @ref benchmarking-results-overhead). Here, both functions take a few nanoseconds per element,
  and the first case checks that the time of each stays under one microsecond. The second case
  takes the four fields with a structured binding, which follows their order of declaration,
  `time`, `fastest`, `slowest`, and `error`:

  @snippet benchmarks/measurement.cpp snippet

  @warning A check on a measurement can fail on a busy machine without any regression. On a
  machine shared with other work, the median time of a row varied by a factor of 2 over 30 runs,
  and the two rows of a speedup are measured one after the other, so a load during one of them
  moves the speedup. An `error` of 0.05 or more marks a row as unstable, while an `error` under
  0.05 does not make it accurate: a load that slows most epochs raises the median and leaves the
  error small, and a single epoch gives 0. A case that leaves out the checks of its unstable rows
  still needs another check, @ref TTS_PASS for instance, or **TTS** reports it as invalid.

  @section benchmarking-results-csv CSV file

  With `-d` or `--dump` on the command line (see @ref cli), a benchmark also writes the times of
  its epochs to `<title>.csv` in the working directory. That file replaces any file of that name,
  such as the file of a previous run or of another benchmark with the same title. The first line
  holds the names of the rows, in quotes. Each following line holds one epoch, with the time per
  element of every row, in seconds. The functions are measured one after the other, so the epochs
  on one line were taken at different times. If the file cannot be opened, for instance because
  the title contains a `/`, a note above the table gives its name. Here are the first lines of
  `hypot.csv`, from the hypot benchmark:

  @code{.unparsed}
  "std::hypot","naive"
  3.11494e-09,1.33064e-09
  3.09032e-09,1.34276e-09
  3.21809e-09,1.39597e-09
  @endcode

  <div class="section_buttons">

  | Previous                                          |                                      Next |
  |:--------------------------------------------------|------------------------------------------:|
  | [Designing a Benchmark](@ref benchmarking-design) | [Custom Traits](@ref benchmarking-traits) |

  </div>
**/
//==================================================================================================
