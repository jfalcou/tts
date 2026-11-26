#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  benchmarking-traits  Custom Traits

  @tableofcontents

  When a library computes on SIMD registers, or on numbers made of several `double` values, the
  functions to benchmark take arguments built from the values of a buffer, and one call may
  process several values at once. The traits of a benchmark, the template parameter of
  tts::bench::basic_benchmark, define the four points that depend on such a library:

  - the input each function runs on,
  - the number of elements one call processes,
  - the function that keeps a result from being optimized away,
  - a line of text printed under the `Assertions` line, such as the instruction set the library
    was built for.

  tts::bench::benchmark is tts::bench::basic_benchmark with tts::bench::default_traits. Under
  these traits, each function runs on its buffers unchanged, and a call processes one element.

  @section benchmarking-traits-input Input

  Derive the traits from tts::bench::default_traits, and redefine only the members that change.
  `input<Row>(values)` is called once on each buffer, before the measurement, and returns what the
  function runs on, by value or by reference. `Row` is the type passed as the second argument of
  @ref TTS_RUN_BENCHMARK. It need not be the type of the values in the buffers, and only the
  traits use it. `environment()` returns a line of text, printed under the
  `Assertions` line. Here, the `double` function runs on the `float` buffer, converted with
  tts::bench::convert, and `environment()` returns a line on the type of the input data. Both
  functions run on the same values and share one table, whereas a
  @ref benchmarking-design-types "template test case" gives each type a table of its own:

  @snippet benchmarks/traits.cpp snippet

  @section benchmarking-traits-lanes Lanes

  `lanes<Row>` is the number of elements that one call processes, shown in the `N` column. The
  time of a row is divided by its number of lanes, so every row shows a time per element. `Eff.`
  is the speedup divided by `N`. When the baseline is the scalar version of the same computation,
  `Eff.` is the parallel efficiency.

  A traits class can also be written without deriving it from tts::bench::default_traits. It then
  has to define all four members: `input`, `lanes`, `prevent_optimization`, and `environment`. Its
  `prevent_optimization(result)` keeps a result from being optimized away, as
  tts::bench::default_traits::prevent_optimization does. Here, the function of the `pack` row
  processes four `float` values per call, from the buffer that the scalar function reads one value
  at a time:

  @snippet benchmarks/lanes.cpp snippet

  @warning The time of a row is divided by its number of lanes, whatever its function computes. If
  the traits give a row `N` lanes and its function processes one element per call, the time per
  element is `N` times too small.

  <div class="section_buttons">

  | Previous                                         | Next |
  |:-------------------------------------------------|-----:|
  | [Reading the Results](@ref benchmarking-results) |      |

  </div>
**/
//==================================================================================================
