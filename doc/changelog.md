Change Log {#changelog}
==========

# Version 4.0 - V.I. Warshawski

## What's Changed

### Breaking Changes
  * `TTS_ABSOLUTE_EQUAL`, `TTS_RELATIVE_EQUAL` and `TTS_ULP_EQUAL` now require both operands to have
    the same type. They used to compare them after promoting both to their common type, which
    measured the distance in the unit of the promoted type rather than the one under test: comparing
    a `float` against its own value as a `double` reported 9.84e+07 ULP instead of 0. A mismatch is
    now a compile-time error naming both types; convert the expected value at the call site.
  * `TTS_RELATIVE_EQUAL` and `TTS_ALL_RELATIVE_EQUAL` report a ratio where they used to report a
    percentage, and the unit they print is `rel` rather than `%`. `abs(L-R) / max(abs(L), abs(R), 1)`
    is compared to the tolerance directly, so every existing tolerance has to be divided by a
    hundred. The factor was the odd one out: `ulp` and `absolute` already reported the quantity
    itself, and a consumer overloading the hook naturally returns the quotient; one that did was
    testing a hundred times looser than its tolerances read.
  * A relative tolerance of one or more is reported as a failure rather than compared: it accepts
    any error up to the value itself, and is a percentage left over from the previous unit. The
    message says to divide it by a hundred. This applies to the native floating point path only; a
    `tts::precision` specialisation keeps whatever unit it returns.
  * Equality, ordering and bitwise checks are unchanged. `TTS_EQUAL` and its typed counterparts
    forward to `operator==` or to a `compare_equal` overload, and introduce no conversion of their
    own.

  * Every customization point is a trait, and the free functions they replace are gone.
    `tts::precision<T>`, `tts::display<T>`, `tts::comparison<L, R>`, `tts::generation<T>` and
    `tts::conversion<T, V>` stand where `ulp_distance`, `relative_distance`, `absolute_distance`,
    `ieee_equal`, `to_text`, `compare_equal`, `compare_less`, `produce` and `convert_as` used to be
    reached by name. Each trait inherits its defaults from a `tts::_::builtin_*` base, so a
    specialization keeps the members it leaves alone by inheriting from that base.
  * An overload of one of the seven customization names left behind is reported where it would have
    been used, rather than ignored in silence. `produce` and `convert_as` are the exception: they
    are the dispatchers rather than customization points, and **TTS** reaches them qualified, so an
    overload of either name in another namespace is never found.

### New Features
  * `tts::is_randoms` and its `tts::is_randoms_v` alias name the range generator by its type. A
    `tts::generation` specialization keys on the type being built and cannot see the generator in
    an overload set, so a range that has to be drawn apart is recognized this way.

# Version 3.0 - Beatrice Adela Bradley

## What's Changed

A large body of work (https://github.com/jfalcou/tts/pull/105) has been done to make **TTS** compile-time far better. This affects the user API a tiny bit, hence the bump to **TTS v3.0**.

### Infrastructure
  * Fix support for MSVC and clang CL (https://github.com/jfalcou/tts/pull/75)
  * **TTS** now use Use [copacabana] as CMake infrastructure library (https://github.com/jfalcou/tts/pull/77)
  * Move documentation to Doxygen (https://github.com/jfalcou/tts/pull/83)
  * Enable WASM tests (https://github.com/jfalcou/tts/pull/95)
  * Enable tests with ICPX (https://github.com/jfalcou/tts/pull/96)

### Bug Fixes
  * Added static_assert to catch unsupported types in TTS_ABSOLUTE/RELATIVE/ULP_EQUAL (https://github.com/jfalcou/tts/pull/78)
  * Fix #82 - Make FATAL error log their messages (https://github.com/jfalcou/tts/pull/85)
  * fixed uint types not in integral_types by **SadiinsoSnowfall** in https://github.com/jfalcou/tts/pull/99
  * Fix #100 - TTS doesn't generate -Wshadow warnings (https://github.com/jfalcou/tts/pull/102)
  * Make produce(sequence) use produce(scalar) to allow for easier non-standard type production by **SadiinsoSnowfall** (https://github.com/jfalcou/tts/pull/110)

### New Features
  * Decorrelate IEEE_EQUAL from ULP_DISTANCE (https://github.com/jfalcou/tts/pull/89)
  * `tts::generate` is deprecated. All data generators are now `constexpr` (https://github.com/jfalcou/tts/pull/105)
  * Added `TTS_BIT_EQUAL` and `TTS_BIT_NOT_EQUAL` to perform bitwise comparison (https://github.com/jfalcou/tts/pull/105)
  * Added `filter` to filter out types in complex case definition (https://github.com/jfalcou/tts/pull/105)
  * Added `cartesian_product` to generate more complex template types list (https://github.com/jfalcou/tts/pull/105)
  * Summary display now displays percentage of success/failures (https://github.com/jfalcou/tts/pull/105)
  * Added the `-q/--quiet` CLI option to remove all output but summary  (https://github.com/jfalcou/tts/pull/105)

### New Contributors
* **SadiinsoSnowfall**

### Detective who?
[Beatrice Adela Bradley](https://en.wikipedia.org/wiki/Mrs_Bradley).

# Version 2.2 - Aubrey Bradimore

## What's Changed

### Infrastructure
  * **We are now under the Boost Software Licence 1.0**
  * Enable Android tests and support
  * Clarify some output when failures occur

### New Features
  * Implement ALL_EQUAL
  * Test can now use configurable, automatic data generator (https://github.com/jfalcou/tts/pull/53)
  * `TTS_EXPECT_COMPILES/COMPILE_NOT` check if a fragment of code can be compiled or not
  * Reimplement `TTS_WHEN` and `TTS_AND_THEN` to support sub-tests
  * Add typed tests that checks both value and exact type

### Bug Fixes
  * Fix Visual Studio interaction with constexpr tests
  * Improve random generators and seed management

## Detective who?
[Aubrey Bradimore](https://en.wikipedia.org/wiki/Aubrey_Bradimore), this is a Christmas Release afterall

# Version 2.1 - Cordelia Gray

## What's Changed
  * Fix #42 - Proper FetchContent support
  * Better compatibility
  * Enable EMSCRIPTEN tests and supports
  * Add tests over sequences

## Detective who?
[Cordelia Gray](https://en.wikipedia.org/wiki/Cordelia_Gray), a very suitable detective

# Version 2.0 - Kay Scarpetta

**TTS** version 2.0 is taking a major break from an earlier version. Compile-time was creeping upward
and some features were not that useful. So the code base got streamlined and simplified.

This version is a **Major Release With API Changes** release:

**Breaking API Changes**:
 + Removal of support for colors
 + Removal of support for sub-scenario
 + Silent mode for tests is the default. The only information displayed concerns failures

**Features:**
  + Improved compile-times
  + Output streamlined to fit machine processing in CI context or scripts
  + Better handling of template test cases: less generated symbol, better flexibility in inputs (list of types or types list generator)

## Detective who?
[Kay Scarpetta](https://en.wikipedia.org/wiki/Kay_Scarpetta), the private forensic consultant.

# Version 1.0 - Phoebe Daring

This version is a **Major Release With API Changes** release:

**Breaking API Changes**:

To support some of the new feature and simplify some macros, **TTS** API is now based on a
lambda system. If this doesn't change the way tests are performed, it now requires to end all
tests cases by a `;`.

**Bug Fixes:**

  * Fix #3  - `TTS_TPL_CASES` now use meta-function based system for types generation
  * Fix #32 - Provide a way to log arbitrary data when failures occur
  * Fix #33 - Provide the `REQUIRED` option on tests to halt tests at first failure
  * Fix #34 - Provide a command-line option to fix precision when displaying floating points
  * Fix #37 - Stop displaying `char const*` as a string.

## Detective who?
[Phoebe Daring](https://en.wikipedia.org/wiki/Phoebe_Daring), the first half of the Daring Twins since 1912.

# Version 0.2 - Kate Fansler

This version is a fix+features release:

  * Fix #20 - Make string and pointers display in a more intuitive way
  * Fix #23 - Add proper parens in `EXPECT` macro
  * Add support for `constexpr` expectation and relational tests
  * `TTS_CASE_TPL` now generates one scenario per type
  * Fix #21 - Add runtime filtering for tests based on description string

## Detective who?
[Kate Fansler](https://en.wikipedia.org/wiki/Kate_Fansler), solving academic crimes since 1964.

# Version 0.1 - Cadfael

## First public release.

**TTS** first complete release enables numerically oriented TDD including:
  - precision testing
  - checks over the data set
  - easy to customize the use of user-defined types.

## Detective who?
[Cadfael](https://en.wikipedia.org/wiki/Cadfael) is probably the eldest of all amateur detectives in history.
