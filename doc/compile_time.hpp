#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  compile-time  Compile-Time Discipline

  @tableofcontents

  @section compile-time-why Why this matters

  TTS is header-only, and it targets test suites - which typically means dozens or hundreds of
  small translation units, each `#include <tts/tts.hpp>`-ing the whole library on its own. There is
  no separately-compiled `.o` to amortize a header's parsing cost over: every header TTS pulls in is
  paid, in full, by every test file, in every project that uses it, on every build. This project's
  own CI matrix alone rebuilds the test suite across more than a dozen compiler/OS/standard-library
  combinations per pull request - a single heavy header added to a commonly-included file is not a
  one-time cost, it is a cost multiplied by every one of those builds, forever.

  This is why TTS reimplements small, purpose-built pieces of what it needs (a minimal string type,
  a monotonic clock, a move-only file wrapper, ...) instead of reaching for the equivalent standard
  library facility: the value proposition of a *tiny* test system depends on it staying cheap to
  include.

  @section compile-time-cost Headers we avoid, and what they cost

  Intuition about which headers are "heavy" is unreliable - some innocuous-looking ones aren't free,
  and some that look expensive on paper measure as free in practice. Only measuring settles it. The
  numbers below come from timing a minimal `TTS_MAIN` translation unit with and without each header
  added, `g++ 13` and `clang++ 18`, `-std=c++20`, best-of-8 runs (see @ref compile-time-checklist for
  the exact method):

  Header           | Added cost (g++) | Added cost (clang++) | Avoided via
  ---------------- | ----------------- | --------------------- | -----------------------------------
  `<chrono>`       | +460ms (~4x)      | +680ms (~6x)          | `tools/clock.hpp`'s `now_ns()`, built on `clock_gettime`/`QueryPerformanceCounter`
  `<regex>`        | +270ms (~3x)      | +350ms (~3.5x)        | not needed anywhere in TTS
  `<thread>`       | +200ms (~2.3x)    | +240ms (~2.8x)        | not needed anywhere in TTS
  `<memory>`       | +170ms (~2x)      | +210ms (~2.5x)        | `tools/file.hpp`'s `file_guard`, a hand-rolled move-only RAII wrapper instead of `std::unique_ptr`
  `<sstream>`      | +140ms            | +190ms                | `tools/text.hpp` builds strings via `malloc`/`snprintf`
  `<iostream>`     | +140ms            | +170ms                | output goes through `FILE*`/`fputs`, not `std::cout`
  `<functional>`   | +100ms            | +100ms                | avoided unless genuinely needed
  `<string>`       | +90ms             | +110ms                | @ref tts::text is TTS's own minimal string type
  `<unordered_map>`| +60ms             | +60ms                 | avoided unless genuinely needed
  `<vector>`       | +50ms             | +60ms                 | avoided unless genuinely needed
  `<map>`          | +50ms             | +50ms                 | avoided unless genuinely needed
  `<array>`        | ~0ms (noise)      | ~0ms (noise)          | fine to use - see below

  @section compile-time-fine What's fine to use

  Being on this page isn't a blanket ban on the standard library - it's a reminder to check before
  assuming. Two examples that already got measured and cleared:

  + `<array>` measures as free (no cost above run-to-run noise) with both compilers above, and is
  used where a bounded, compile-time-sized container earns its keep.
  + `<windows.h>`, guarded by `WIN32_LEAN_AND_MEAN` and `NOMINMAX` and included only on the Windows
  branch of `tools/clock.hpp`, was measured on real `cl.exe` and found free relative to an empty
  translation unit - lighter, in fact, than the fragile forward-declare-the-three-functions-we-need
  workaround that seemed safer on paper. Platform-gated code still gets measured on that platform,
  not assumed safe or assumed expensive.

  @section compile-time-checklist Before you add a header

  1. Check whether what you need is already available. `engine/deps.hpp` is included by essentially
  every TTS header and already pulls in `<cstdio>`, `<cstdlib>`, `<cstring>`, `<type_traits>`,
  `<utility>`, `<initializer_list>`, `<cassert>`, `<bit>`, `<compare>`, `<concepts>`, `<cstdint>`,
  `<limits>` and `<new>` - reusing one of those costs nothing further.
  2. If a new header genuinely seems necessary, measure it, don't guess. Write two near-identical
  translation units, one with the header and one without, and time both:
  @code{sh}
  time g++ -std=c++20 -I include -c file.cpp -o /dev/null
  @endcode
  Repeat several times (5-8 runs is usually enough to see past scheduler noise) and compare the best
  or average time against the baseline. Do this on more than one compiler if you can - the table
  above shows the same header can cost meaningfully different amounts on `g++` versus `clang++`.
  3. For a change spread across several files, `git worktree add ../tts-baseline main` gives you an
  untouched copy of `main` to diff against without disturbing your own working tree - build and time
  the same representative translation unit from both trees.
  4. Report the actual numbers in the pull request. "I checked and it's fine" asks a reviewer to
  either trust that blindly or redo the measurement themselves; the before/after numbers let them
  just read the answer.

  @section compile-time-precedent Precedent in this codebase

  A few places in TTS exist specifically because of a header that didn't clear this bar - useful
  reading before writing a new one from scratch:

  + `tools/clock.hpp` - a portable monotonic-nanosecond timer, not `std::chrono`.
  + `tools/file.hpp` - `file_guard`, a move-only RAII `FILE*` wrapper, not `std::unique_ptr`.
  + `tools/text.hpp` - `tts::text`, a minimal `malloc`/`snprintf`-based string type, not `std::string`.
**/
//==================================================================================================
