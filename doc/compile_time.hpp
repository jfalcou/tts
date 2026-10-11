#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  compile-time  Compile-Time Cost

  @tableofcontents

  @section compile-time-headers Standard headers in TTS

  @subsection compile-time-why Include cost of a test suite

  The library is header-only, and a test suite holds dozens or hundreds of small translation units,
  each including `<tts/tts.hpp>` on its own. No separately compiled object amortizes the parsing, so
  each header included by **TTS** is parsed once per test file and per build, in each project using
  the library. The CI matrix of the library alone rebuilds the suite on more than a dozen
  combinations of compiler, system, and standard library per pull request, so a header added to a
  widely included file adds its parsing time to each of those builds.

  The library therefore holds small replacements for the facilities it needs, listed in
  @ref compile-time-precedent, rather than including the standard headers.

  @subsection compile-time-cost Avoided headers and their cost

  The cost of a header is measured before the header is included, since the name of a header does
  not predict its cost. The times below are what a header adds to a minimal `TTS_MAIN` translation
  unit, with `g++ 13` and `clang++ 18`, `-std=c++20`, best of eight runs, measured as
  @ref compile-time-checklist describes.

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

  @subsection compile-time-fine Headers measured as free

  A standard header is allowed once measured as free. `<windows.h>` is included only on the Windows
  branch of `tools/clock.hpp`, guarded by `WIN32_LEAN_AND_MEAN` and `NOMINMAX`. Measured on
  `cl.exe`, it costs nothing relative to an empty translation unit, and less than declaring by hand
  the three functions it provides. Platform-specific code is measured on that platform.

  @subsection compile-time-checklist Checklist for a new header

  1. `engine/deps.hpp`, included by nearly every **TTS** header, already includes `<cstdio>`,
  `<cstdlib>`, `<cstring>`, `<type_traits>`, `<utility>`, `<initializer_list>`, `<cassert>`,
  `<bit>`, `<cstddef>`, `<compare>`, `<concepts>`, `<cstdint>`, `<limits>`, and `<new>`. Using one
  of them costs nothing more.
  2. A new header is measured on two near-identical translation units, one with the header and one
  without:
  @code{sh}
  time g++ -std=c++20 -I include -c file.cpp -o /dev/null
  @endcode
  Five to eight runs are taken to amortize scheduling effects; the best time is compared with the
  baseline. The same header costs different amounts with `g++` and `clang++` (`<chrono>` adds 460 ms
  with the first and 680 ms with the second), so both are measured.
  3. A change spread across several files is measured against an untouched copy of `main`, created
  with `git worktree add ../tts-baseline main`, by building the same representative translation
  unit in both trees.
  4. The pull request gives the numbers before and after the change.

  @subsection compile-time-precedent Replacements written in TTS

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

  The CPU time of compiling one test unit with `-std=c++20 -c` is measured at each publication of
  this documentation, for **TTS**, doctest, Catch2, GoogleTest, Boost.Test, and ut. Each point is
  the median of several compilations.

  @htmlinclude bench-meta.html

  Catch2 and GoogleTest compile their runner in a separate library, whose compilation time is not
  measured. The runner of doctest, Boost.Test, ut, and **TTS** is compiled in the unit that defines
  it.

  @subsection compile-time-comparison-include Minimal include cost

  The unit holds the test runner and no test case.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-include-gcc.html

  @tab{clang++}
  @htmlinclude bench-include-clang.html

  @tab_end

  @subsection compile-time-comparison-cases Cost of test cases

  The unit holds a growing number of test cases. A simple case runs three checks on two integers,
  and a template case runs the same three checks over `short`, `int`, `float`, and `double`.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-cases-gcc.html

  @tab{clang++}
  @htmlinclude bench-cases-clang.html

  @tab_end

  @subsection compile-time-comparison-types Template machinery

  Two units are compared at the largest N measured. The first holds N test cases, each on a type
  of its own, and the second a single template test case over the same N types. The difference
  between the two bars is the cost of the template machinery alone, for each library.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-types-gcc.html

  @tab{clang++}
  @htmlinclude bench-types-clang.html

  @tab_end

  @subsection compile-time-comparison-fits Fixed cost and crossings

  Each curve is fitted by least squares to a fixed cost plus a cost per test case. The second table
  gives the number of test cases in a unit where **TTS** compiles faster:

  - `up to N`: the other library costs less per case, and its line crosses that of **TTS** at N;
  - `from N`: the other library costs more per case, and its line crosses that of **TTS** at N;
  - `always` or `never`: the two lines do not cross at a positive number of cases.

  Catch2 and GoogleTest are measured without their runner, so **TTS** compiles faster over at least
  the range given against them.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-fits-gcc.html

  @tab{clang++}
  @htmlinclude bench-fits-clang.html

  @tab_end
**/
//==================================================================================================
