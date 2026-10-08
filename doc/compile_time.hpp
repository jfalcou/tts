#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  compile-time  Compile-Time Discipline

  @tableofcontents

  @section compile-time-headers Headers of TTS

  @subsection compile-time-why Include cost of a test suite

  The library is header-only, and a test suite holds dozens or hundreds of small translation units, each
  including `<tts/tts.hpp>` on its own. No separately compiled object amortizes the parsing: every
  header **TTS** pulls in is parsed again by every test file, in every project using it, at every
  build. The CI matrix of this project alone rebuilds the suite on more than a dozen combinations of
  compiler, system and standard library per pull request, so a heavy header added to a widely
  included file multiplies its cost by every one of those builds.

  The library therefore writes small replacements for what it needs, such as a minimal string type, a
  monotonic clock or a move-only file wrapper, rather than include the standard facility.

  @subsection compile-time-cost Avoided headers and their cost

  The cost of a header is measured, never assumed: some light-looking headers cost a lot, and some
  heavy-looking ones cost nothing. The table gives the time a header adds to a minimal `TTS_MAIN`
  translation unit, with `g++ 13` and `clang++ 18`, `-std=c++20`, best of 8 runs, measured as
  @ref compile-time-checklist describes.

  Header           | g++               | clang++               | Avoided via
  ---------------- | ----------------- | --------------------- | -----------------------------------
  `<chrono>`       | +460ms (~4x)      | +680ms (~6x)          | `tools/clock.hpp`'s `now_ns()`, built on `clock_gettime`/`QueryPerformanceCounter`
  `<memory>`       | +170ms (~2x)      | +210ms (~2.5x)        | `tools/file.hpp`'s `file_guard`, a hand-rolled move-only RAII wrapper instead of `std::unique_ptr`
  `<sstream>`      | +140ms            | +190ms                | `tools/text.hpp` builds strings via `malloc`/`snprintf`
  `<iostream>`     | +140ms            | +170ms                | output goes through `FILE*`/`fputs`, not `std::cout`
  `<functional>`   | +100ms            | +100ms                | never: `tools/erased.hpp`'s type-erased `callable` wraps every `TTS_CASE` body, the hottest path of the library
  `<string>`       | +90ms             | +110ms                | @ref tts::text is TTS's own minimal string type
  `<unordered_map>`| +60ms             | +60ms                 | avoided unless genuinely needed
  `<vector>`       | +50ms             | +60ms                 | `tools/buffer.hpp`'s `tts::buffer`, a minimal `malloc`-based dynamic array
  `<map>`          | +50ms             | +50ms                 | avoided unless genuinely needed
  `<regex>`        | +270ms (~3x)      | +350ms (~3.5x)        | not needed so far
  `<thread>`       | +200ms (~2.3x)    | +240ms (~2.8x)        | not needed so far
  `<array>`        | +20ms             | +20ms                 | C arrays, sized by a constant

  @subsection compile-time-fine Measured and cleared headers

  A standard header stays allowed once measured as free. `<windows.h>`, guarded by
  `WIN32_LEAN_AND_MEAN` and `NOMINMAX` and included only on the Windows branch of `tools/clock.hpp`,
  measured on `cl.exe` as free relative to an empty translation unit, and lighter than declaring the
  three functions it provides by hand. Code reserved to a platform is measured on that platform.

  @subsection compile-time-checklist Checklist for a new header

  1. `engine/deps.hpp`, which nearly every **TTS** header includes, already brings `<cstdio>`,
  `<cstdlib>`, `<cstring>`, `<type_traits>`, `<utility>`, `<initializer_list>`, `<cassert>`, `<bit>`,
  `<compare>`, `<concepts>`, `<cstdint>`, `<limits>` and `<new>`. Using one of them costs nothing more.
  2. A new header is measured on two near-identical translation units, one with the header and one
  without:
  @code{sh}
  time g++ -std=c++20 -I include -c file.cpp -o /dev/null
  @endcode
  Five to eight runs see past the scheduler noise; the best or the average time is compared with the
  baseline. The same header costs different amounts with `g++` and `clang++`, as the table shows, so
  both are measured.
  3. A change spread across several files is measured against an untouched copy of `main`, such as
  `git worktree add ../tts-baseline main`, by building the same representative translation unit in
  both trees.
  4. The pull request gives the numbers before and after the change.

  @subsection compile-time-precedent Replacements written in TTS

  These files exist because a standard header did not clear the bar, and serve as models for a new
  replacement:

  + `tools/clock.hpp`: a portable monotonic timer in nanoseconds, in place of `std::chrono`.
  + `tools/file.hpp`: `file_guard`, a move-only RAII `FILE*` wrapper, in place of `std::unique_ptr`.
  + `tools/text.hpp`: `tts::text`, a minimal string type built on `malloc` and `snprintf`, in place of
  `std::string`.

  @section compile-time-comparison Other test libraries

  The charts give the CPU time of compiling one test unit with `-std=c++20 -c`, with **TTS** and with
  doctest, Catch2, GoogleTest, Boost.Test and ut, measured again at each build of this documentation.
  Every point is the median of several compilations.

  @htmlinclude bench-meta.html

  Catch2 and GoogleTest compile their runner in a library of their own, which the charts do not
  count. doctest, Boost.Test, ut and **TTS** compile it in the unit that defines it.

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
  and a template case runs the same three checks over `short`, `int`, `float` and `double`.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-cases-gcc.html

  @tab{clang++}
  @htmlinclude bench-cases-clang.html

  @tab_end

  @subsection compile-time-comparison-types Template machinery

  The unit holds N test cases, each on a type of its own, then a single template test case over the
  same N types, with the largest N measured. The gap between the two bars is the cost of the
  template machinery of each library alone.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-types-gcc.html

  @tab{clang++}
  @htmlinclude bench-types-clang.html

  @tab_end

  @subsection compile-time-comparison-fits Fixed cost and crossings

  Each curve is fitted by least squares as a fixed cost and a cost per test case. Where another
  library costs less per case than **TTS**, its line crosses the one of **TTS** at the number of cases
  the second table gives; `always` means it never does. Against Catch2 and GoogleTest, the crossing
  is a lower bound, their runner being left out.

  @tab_begin

  @tab{g++}
  @htmlinclude bench-fits-gcc.html

  @tab{clang++}
  @htmlinclude bench-fits-clang.html

  @tab_end
**/
//==================================================================================================
