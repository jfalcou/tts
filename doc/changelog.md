Change Log {#changelog}
==========

# Upcoming version

## What's Changed

### New Features
  * Added `tts::sweep` to run a range check on every value of a type, or on a sample when there are more than `--scope` values (https://github.com/jfalcou/tts/pull/279)
  * Added `tts::enumeration<T>` to enumerate the values of a type for `tts::sweep` (https://github.com/jfalcou/tts/pull/279)
  * Added `tts::sweep(n)` and `tts::sweep(lo, hi, n)` to give one range check its own scope of `n` calls (https://github.com/jfalcou/tts/pull/279)
  * Range checks no longer keep all their inputs and results in memory (https://github.com/jfalcou/tts/pull/279)

### Bug Fixes
  * Rewrote the reference entries of the test macros, which cited functions removed in 4.0 and gave wrong briefs (https://github.com/jfalcou/tts/pull/280)


# Version 4.1

## What's Changed

### New Features
  * Fix #266 - Run a test body at run time and in constant evaluation (https://github.com/jfalcou/tts/pull/267)
    - `TTS_CONSTEXPR("...")` on the title of a `TTS_CASE` or a `TTS_CASE_TPL` runs the body at run time and in constant evaluation. `TTS_CONSTEVAL("...")` runs it in constant evaluation only.
    - Both nest with `TTS_XFAIL`, `TTS_MAYFAIL`, `TTS_XINVALID`, and `TTS_TIMEOUT` in either order.
    - `TTS_CASE_IN` and `TTS_CASE_TPL_IN` take `tts::runtime`, `tts::constant`, or both, and run the body once per context in the order given.
    - The body of a `TTS_CASE_TPL` with a wrapped title, or of a `TTS_CASE_TPL_IN`, declares its parameter as `<typename T>(TTS_CONTEXT(tts::type<T>))`.
    - A failure in constant evaluation reports its expression, file, and line, and the compared values when the case also runs at run time.
    - The basic, relation, and type checks run in constant evaluation, with their typed and constexpr forms. The precision, sequence, range, and exception checks fail to compile there.
    - A `TTS_CASE_WITH` titled with `TTS_CONSTEXPR` or `TTS_CONSTEVAL` fails on a `static_assert`.
  * `TTS_ULP_RANGE_CHECK` accepts functions of several arguments (https://github.com/jfalcou/tts/pull/260)
    - `RefType` and `NewType` list the argument types in call order as `tts::types`, and `Producer` holds one generator per argument between parentheses, or one for all.
  * `TTS_ULP_RANGE_CHECK` compares a `std::pair`, a `std::tuple`, or another tuple-like result member by member, with one bound for all members or one per member (https://github.com/jfalcou/tts/pull/260)
  * `TTS_ULP_RANGE_CHECK` prints its histogram only on failure or under `--verbose` (https://github.com/jfalcou/tts/pull/260)
  * The report of `TTS_ULP_RANGE_CHECK` writes each function as a call, `ref(float, int)`, and puts stars around the sampled element of a `NewType` that holds several values (https://github.com/jfalcou/tts/pull/260)
  * `tts::adapter<T>` replaces `run`, `retrieve`, and `display` with `element_type`, `size`, `load`, and `store`, and a 4.0 adapter fails on a `static_assert` that names them (https://github.com/jfalcou/tts/pull/260)
  * Removed `tts::ulp_histogram`: run `TTS_ULP_RANGE_CHECK` under `--verbose` to print the histogram (https://github.com/jfalcou/tts/pull/260)
  * The generators of `TTS_ULP_RANGE_CHECK` receive their index and count as `std::ptrdiff_t` instead of `std::size_t` (https://github.com/jfalcou/tts/pull/260)
  * Reduced the compile time of a test to about a quarter with g++ 15 and clang++ 19 (https://github.com/jfalcou/tts/pull/253)
  * Added the CMake option `TTS_EXTERNAL_DEPENDENCIES`, `OFF` by default, to take copacabana from `COPACABANA_SOURCE_DIR` instead of downloading it through CPM (https://github.com/jfalcou/tts/pull/276)

### Bug Fixes
  * Fix #255 - Disable `-Wc2y-extensions` in every translation unit that includes TTS under clang 22 and later, except on Apple platforms, so `TTS_CASE` compiles under `-Wpedantic -Werror` (https://github.com/jfalcou/tts/pull/256)
  * Fix `-Wshadow` warnings when a test variable has the name of a local of a check macro (https://github.com/jfalcou/tts/pull/247)
  * `tts::buffer` allocates through `malloc` except for over-aligned types, so a test that replaces `operator new` to count its allocations no longer counts those of `tts::buffer` (https://github.com/jfalcou/tts/pull/248)
  * A passing `TTS_THROW` or `TTS_NO_THROW` counts once in the summary (https://github.com/jfalcou/tts/pull/253)
  * A failing `TTS_GREATER_EQUAL` and its typed and constexpr forms print `<` as their negation instead of `<=` (https://github.com/jfalcou/tts/pull/253)
  * A typed constexpr check, such as `TTS_TYPED_CONSTEXPR_EQUAL`, that fails on a type mismatch prints the message streamed after it (https://github.com/jfalcou/tts/pull/253)

### Infrastructure
  * Run the nightly CI matrix on demand (https://github.com/jfalcou/tts/pull/262)
  * Added a [compile-time comparison](https://jfalcou.github.io/tts/compile-time.html) with doctest, Catch2, GoogleTest, Boost.Test, and ut to the documentation (https://github.com/jfalcou/tts/pull/245, https://github.com/jfalcou/tts/pull/252, https://github.com/jfalcou/tts/pull/254, https://github.com/jfalcou/tts/pull/257, https://github.com/jfalcou/tts/pull/259, https://github.com/jfalcou/tts/pull/261)
  * Move the CI to the compilers:v11 image, with clang 22 (https://github.com/jfalcou/tts/pull/256)
  * Move the macOS jobs to macos-15 (https://github.com/jfalcou/tts/pull/265)
  * Rebuild and redeploy the documentation on demand (https://github.com/jfalcou/tts/pull/272)
  * Check the CMake files with cmake-format and cmake-lint, and the workflows with actionlint, in pre-commit (https://github.com/jfalcou/tts/pull/273)

# Version 4.0 - V.I. Warshawski

## What's Changed

### Breaking Changes
  * `tts::reverse_ramp` counts down to `start` instead of from it, so `reverse_ramp{0}` gives no negative value (https://github.com/jfalcou/tts/pull/229)
  * `TTS_ABSOLUTE_EQUAL`, `TTS_RELATIVE_EQUAL`, and `TTS_ULP_EQUAL` require both operands to have the same type (https://github.com/jfalcou/tts/pull/229)
  * `TTS_RELATIVE_EQUAL` and `TTS_ALL_RELATIVE_EQUAL` measure a ratio instead of a percentage: divide existing tolerances by 100 (https://github.com/jfalcou/tts/pull/229)
  * `TTS_RELATIVE_EQUAL` and `TTS_ALL_RELATIVE_EQUAL` fail on an arithmetic type given a finite tolerance of 1 or more (https://github.com/jfalcou/tts/pull/229)
  * Replaced the customization functions with traits (https://github.com/jfalcou/tts/pull/229)
    - `tts::precision<T>` for `ulp_distance`, `relative_distance`, `absolute_distance`, and `ieee_equal`
    - `tts::display<T>` for `to_text`
    - `tts::comparison<L, R>` for `compare_equal` and `compare_less`
    - `tts::generation<T>` for an overload of `produce`
    - `tts::conversion<T, V>` for an overload of `convert_as`
  * A leftover overload of a former customization function fails to compile where it is used, and a leftover `produce` or `convert_as` outside namespace `tts` is ignored (https://github.com/jfalcou/tts/pull/229)
  * Values have not been printed through `operator<<` since 3.0: specialize `tts::display<T>` for them (https://github.com/jfalcou/tts/pull/229)
  * `<tts/tts.hpp>` brings the built-in sinks only into the translation unit that defines `TTS_MAIN`: include `<tts/sinks/sinks.hpp>` to use one elsewhere (https://github.com/jfalcou/tts/pull/244)
  * `<tts/tts.hpp>` no longer includes `<array>` (https://github.com/jfalcou/tts/pull/244)

### New Features
  * A specialization of `tts::comparison<L, R>::bit_equal` customizes `TTS_BIT_EQUAL` and `TTS_BIT_NOT_EQUAL`, which default to `memcmp` (https://github.com/jfalcou/tts/pull/229)
  * Added `tts::is_randoms` and `tts::is_randoms_v` to detect a `tts::randoms` generator by its type (https://github.com/jfalcou/tts/pull/229)

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
