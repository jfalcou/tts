#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  cli Command Line Interface

  @tableofcontents

  # Environment Variable Fallback

  Every long-form option below (`--xxx` or `--xxx=value`) can also be set via a matching
  `TTS_XXX` environment variable instead of a command line flag - derived by uppercasing the
  flag name and replacing `-` with `_`. Short forms (`-v`, `-q`, ...) have no environment
  variable equivalent. A CLI flag always takes precedence over its environment variable when
  both are set.

  This exists mainly for CI systems where the test command itself is fixed (e.g. `ctest`
  invokes every registered binary through a command line set once at configure time) and
  can't easily template a different flag value per parallel job, but can set a different
  environment variable per job instead:

  @code{sh}
  TTS_SHARD=0/4 ./my_test   # same as ./my_test --shard=0/4
  @endcode

  Environment Variable | Equivalent Option
  --------------------- | -----------------
  `TTS_HELP`            | `--help` / `-h`
  `TTS_HEX`             | `--hex` / `-x`
  `TTS_SCIENTIFIC`      | `--scientific` / `-s`
  `TTS_VERBOSE`         | `--verbose` / `-v`
  `TTS_QUIET`           | `--quiet` / `-q`
  `TTS_DRY`             | `--dry`
  `TTS_ALLOW_EMPTY`     | `--allow-empty`
  `TTS_CAPTURE`         | `--capture=path`
  `TTS_SHARD`           | `--shard=i/n`
  `TTS_SINK`            | `--sink=name`
  `TTS_PRECISION`       | `--precision=N`
  `TTS_SEED`            | `--seed=N`
  `TTS_BLOCK`           | `--block=N`
  `TTS_LOOP`            | `--loop=N`
  `TTS_ULPMAX`          | `--ulpmax=N`
  `TTS_VALMIN`          | `--valmin=N`
  `TTS_VALMAX`          | `--valmax=N`

  # Display Options

  The following options modify how the tests are run and the results displayed.

  Options         | Shortcut | Description
  --------------- | -------- | ---------------------------------------------------------------
  `--help`        | `-h`     | Display the list of supported options `./my_test --help`
  `--hex`         | `-x`     | Display floating point values as hexadecimal. `./my_test --hex`
  `--scientific`  | `-s`     | Display floating point values as hexadecimal. `./my_test --scientific`
  `--verbose`     | `-v`     | Display tests results regardless of their status. `./my_test --verbose`
  `--quiet`       | `-q`     | Remove all tests results regardless of their status. `./my_test --quiet`

  # Execution Control
  These options change what actually gets run and how results are reported, mostly useful for
  CI integration. Those options have no short form.

  Options           | Description
  ----------------- | -----------------------------------------------------------------------
  `--dry`           | Print registered test names without running them. `./my_test --dry`
  `--allow-empty`   | Do not fail when the test suite registered zero test. `./my_test --allow-empty`
  `--capture=path`  | Capture this run's output and write it to `path` instead of stdout. `./my_test --capture=report.txt`
  `--shard=i/n`     | Only run the tests in shard `i` of `n` (`0 <= i < n`). `./my_test --shard=1/3`
  `--sink=name`     | Format output as `name` (`colored`, `tap`, or `diagnostics`) - see @ref output-sinks. `./my_test --sink=tap`

  @note `--shard=i/n` partitions tests by registration index, round-robin (test at index `k`
  belongs to shard `k % n`), not by contiguous blocks - this keeps shards balanced even when
  slow tests cluster together in source order. A shard landing on zero tests (e.g. `n` larger
  than the suite's size) is not treated as a build error: `--shard` implicitly behaves like
  `--allow-empty` for that case. It composes with `--dry`, which then only lists the tests
  that shard would actually run. Like `--sink`, it's a no-op for binaries using a
  @ref TTS_CUSTOM_DRIVER_FUNCTION - those typically assert a fixed, whole-suite fail/invalid
  count via `tts::report(fails, invalids)` from outside the driver loop, and round-robin
  partitioning has no way to know how many of that fixed total fall in a given shard, so
  shard-filtering them could spuriously pass or fail. See #187 for a possible future per-test
  expected-outcome mechanism that would lift this restriction.

  `--dry` prints registered test names without running anything - here's exactly what it does and
  doesn't cover.

  @note `--dry` only prints what's known without running any test: registered @ref TTS_CASE /
  @ref TTS_CASE_TPL / @ref TTS_CASE_WITH names, plus their types for the latter two. It cannot
  list @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since those only exist as runtime control
  flow inside a test's body - which `--dry` never executes. For example, given:

  @code{cpp}
  TTS_CASE_TPL("Check stack behavior", std::int32_t, double)
  <typename T>(::tts::type<T>)
  {
    TTS_WHEN("a stack of T is empty")
    {
      TTS_AND_THEN("push increases its size") { };
      TTS_AND_THEN("pop is a no-op")          { };
    };
  };
  @endcode

  `./my_test --dry` prints the case name with its types, but the `WHEN`/`AND_THEN`
  sub-scenarios stay invisible:

  @code{sh}
  Check stack behavior <int, double>
  @endcode

  # Tests Parameters
  These options are provided to control the specifics of the tests parameters. Those
  options require a value and have no short form.

  Options          | Description
  ---------------- | -----------------------------------------------------------------------
  `--precision=N`  | Set the precision for displaying floating point values `./my_test --precision=12`
  `--seed=N`       | Set the PRNG seeds (default is time-based) `./my_test --valmin=-100`

  # Range Parameters
  This set of options controls the specifics of the range check tests. Those
  options require a value and have no short form.

  Options      | Description
  ------------ | -----------------------------------------------------------------------
  `--block=N`  | Set size of range checks samples to `N`. `./my_test --block=4096`
  `--loop=N`   | Repeat each range checks `N` times. `./my_test --loop=100`
  `--ulpmax=N` | Set global failure ulp threshold for range tests to `N`. `./my_test --ulpmax=4`
  `--valmin=N` | Set minimal value for range tests to `N`. `./my_test --valmin=-100`
  `--valmax=N` | Set maximal value for range tests to `N`. `./my_test --valmax=1000`

**/
//==================================================================================================
