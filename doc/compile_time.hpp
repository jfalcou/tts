#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  compile-time  Compile-Time Cost

  @tableofcontents

  @section compile-time-headers Standard headers in TTS

  @subsection compile-time-why Objectives

  The library is header-only, and a test suite holds dozens or hundreds of small translation units,
  each including `<tts/tts.hpp>` on its own. No separately compiled object amortizes the parsing, so
  each header included by **TTS** is parsed once per test file and per build, in each project using
  the library. The CI matrix of the library alone rebuilds the suite on more than a dozen
  combinations of compiler, system, and standard library per pull request, so a header added to a
  widely included file adds its parsing time to each of those builds.

  The library therefore holds small replacements for the facilities it needs, listed in
  @ref compile-time-precedent, rather than including the standard headers.

  @subsection compile-time-cost Costly headers

  The cost of a header is measured before the header is included, since the name of a header does
  not predict its cost. The times below are what a header adds to a minimal `TTS_MAIN` translation
  unit, with `g++ 13` and `clang++ 18`, `-std=c++20`, best of eight runs.

  Header            | g++    | clang++ | Replacement
  ----------------- | ------ | ------- | ----------------------------------------------
  `<chrono>`        | +460ms | +680ms  | `now_ns()` in `tools/clock.hpp`
  `<regex>`         | +270ms | +350ms  | none, not used
  `<thread>`        | +200ms | +240ms  | none, not used
  `<memory>`        | +170ms | +210ms  | `file_guard` in `tools/file.hpp`
  `<sstream>`       | +140ms | +190ms  | `tts::text` in `tools/text.hpp`
  `<iostream>`      | +140ms | +170ms  | `fputs` on a `FILE*`, in place of `std::cout`
  `<functional>`    | +100ms | +100ms  | `callable` in `tools/erased.hpp`
  `<string>`        | +90ms  | +110ms  | @ref tts::text in `tools/text.hpp`
  `<unordered_map>` | +60ms  | +60ms   | none, not used
  `<vector>`        | +50ms  | +60ms   | `tts::buffer` in `tools/buffer.hpp`
  `<map>`           | +50ms  | +50ms   | none, not used
  `<array>`         | +20ms  | +20ms   | a C array sized by a constant

  @subsection compile-time-fine Acceptable headers

  A standard header is allowed once measured as free:

  - two translation units, identical but for the header, are compiled with `-std=c++20 -c` five to
    eight times each, and their best times are compared;
  - `g++` and `clang++` are both measured, since the same header costs them differently;
  - the pull request that adds the header gives both times.

  `<windows.h>` is included only on the Windows branch of `tools/clock.hpp`, guarded by
  `WIN32_LEAN_AND_MEAN` and `NOMINMAX`. Measured on `cl.exe`, it costs nothing relative to an empty
  translation unit, and less than declaring by hand the three functions it provides.
  Platform-specific code is measured on that platform.

  `engine/deps.hpp`, included by nearly every **TTS** header, already includes the acceptable
  headers, so using one of them costs nothing more.

  @subsection compile-time-precedent TTS replacements

  Each file below replaces a standard header listed in the table above, and is a model for a new
  replacement:

  + `tools/clock.hpp`: `now_ns()`, a portable monotonic timer in nanoseconds built on
  `clock_gettime` or `QueryPerformanceCounter`, in place of `std::chrono`.
  + `tools/file.hpp`: `file_guard`, a move-only RAII `FILE*` wrapper, in place of `std::unique_ptr`.
  + `tools/text.hpp`: `tts::text`, a minimal string type built on `malloc` and `snprintf`, in place
  of `std::string` and the string streams of `<sstream>`.
  + `tools/buffer.hpp`: `tts::buffer`, a minimal `malloc`-based dynamic array, in place of
  `std::vector`.
  + `tools/erased.hpp`: `callable`, a type-erased wrapper that stores each `TTS_CASE` body, in
  place of `std::function`.

  @section compile-time-comparison Other test libraries

  Each publication of this documentation builds test executables with **TTS**,
  [doctest](https://github.com/doctest/doctest),
  [Catch2](https://github.com/catchorg/Catch2),
  [GoogleTest](https://github.com/google/googletest),
  [Boost.Test](https://github.com/boostorg/test), and
  [ut](https://github.com/boost-ext/ut), and measures the CPU time of their compilation and link
  with `-std=c++20`. Each executable runs and passes before its time is kept, and each point is the
  median of several builds. Each publication runs on a new CI machine, named below, and its times
  compare only with one another.

  @htmlinclude bench-meta.html

  Every test is the same trivial case, three checks on two integers. Each library is set up as its
  documentation sets it up:

  - **TTS**, [doctest](https://github.com/doctest/doctest), and
    [Boost.Test](https://github.com/boostorg/test) compile their runner in the unit that defines
    `TTS_MAIN`, defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`, or includes
    `boost/test/included/unit_test.hpp`: the test file of a one-file executable, and a `main.cpp` of
    its own otherwise.
  - [ut](https://github.com/boost-ext/ut) runs its tests from its header, so every test file holds its
    runner.
  - [Catch2](https://github.com/catchorg/Catch2) and
    [GoogleTest](https://github.com/google/googletest) link `Catch2WithMain` and `gtest_main`, built
    once per build tree and left out of the executables.
  - [doctest](https://github.com/doctest/doctest) checks with its decomposing `CHECK(a == b)`; its
    binary `CHECK_EQ(a, b)` costs less per test.

  @subsection compile-time-comparison-include Include cost

  A test file holding neither runner nor test, compiled with `-c`. The unit of
  [ut](https://github.com/boost-ext/ut) holds its runner.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-include-gcc.html

  @tab{clang++}
  @htmlinclude bench-include-clang.html

  @tab_end

  @subsection compile-time-comparison-one One test

  An executable built from one file holding the runner and one test.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-one-gcc.html

  @tab{clang++}
  @htmlinclude bench-one-clang.html

  @tab_end

  @subsection compile-time-comparison-cases Tests in one file

  An executable built from one file holding the runner and a growing number of tests. Each curve is
  fitted by least squares to a fixed cost plus a cost per test.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-cases-gcc.html
  @htmlinclude bench-fits-gcc.html

  @tab{clang++}
  @htmlinclude bench-cases-clang.html
  @htmlinclude bench-fits-clang.html

  @tab_end

  @subsection compile-time-comparison-tests Cost of one test

  A test file without runner holding 200 tests, compiled with `-c`. The cost of one test is its time
  minus that of the test file with no test, divided by the number of tests. The runner stays out:
  for some libraries, it varies more between two builds than 50 tests add to the curve above.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-tests-gcc.html

  @tab{clang++}
  @htmlinclude bench-tests-clang.html

  @tab_end

  @subsection compile-time-comparison-files Tests in many files

  An executable built from a runner and a growing number of files of one test each, linked together.
  **TTS**, [doctest](https://github.com/doctest/doctest), and
  [Boost.Test](https://github.com/boostorg/test) compile their runner once, in its own file;
  [ut](https://github.com/boost-ext/ut) compiles it in each file.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-files-gcc.html

  @tab{clang++}
  @htmlinclude bench-files-clang.html

  @tab_end
**/
//==================================================================================================
