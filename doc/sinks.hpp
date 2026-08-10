#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  output-sinks Built-in Output Sinks

  @tableofcontents

  # Overview

  TTS has an extensible @ref tts::output_sink customization point (see @ref tools-output and the
  `tts::gathering_sink` example) - everything a test suite prints goes through whichever sink is
  currently installed. Beyond writing your own, TTS ships a small set of ready-to-use sinks under
  `include/tts/sinks/`, already available through `#include <tts/tts.hpp>` with no extra include
  needed.

  All four draw from @ref tts::output_sink's structured hooks (`test_started()`,
  `assertion_failed()`, `test_finished()`, `suite_finished()`, `suite_metric()`,
  `suite_aborted()`) rather than by parsing the text `tts::stdout_sink` prints, so they stay
  correct regardless of `-v`/`-q`. None of them currently reflect @ref TTS_CASE_TPL's per-type
  breakdown or @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since those don't have their own
  hook.

  @note On a plain @ref TTS_MAIN binary (no custom driver), the `--sink=name` CLI flag installs
  any of the four below without writing any C++ at all - `--sink=colored`, `--sink=tap`,
  `--sink=diagnostics`, or `--sink=json`. It composes with `--capture=path`:
  `--sink=tap --capture=report.tap` writes TAP-formatted output to the file instead of stdout.
  Like `--shard`, it's a no-op for binaries using a @ref TTS_CUSTOM_DRIVER_FUNCTION, which
  already manages its own sink - see @ref cli for the full flag reference.

  # Simple Format Sinks

  Each of these produces plain, line-oriented text - as opposed to a structured-format sink like
  @ref tts::json_sink below, which assembles a single well-formed document with its own schema
  instead of just lines of text.

  ## %tts::colorized_sink

  Wraps a target sink (`tts::output_handler::default_sink()` by default), coloring pass/fail/
  invalid lines and the `Results: ...` summary with ANSI escapes - forwarding everything else
  unchanged. Opt-in: not every terminal or CI log renders ANSI escapes usefully, and on Windows
  it additionally depends on the host console having Virtual Terminal Processing enabled (most
  modern terminals already do).

  @code{cpp}
  tts::colorized_sink colorized;
  tts::output().sink(colorized);

  // ... run the test suite ...
  @endcode

  Or, without a custom driver: `./my_test --sink=colored`.

  A run with one passing, one invalid and one failing case then looks like this:

  @htmlonly
  <pre style="background-color:var(--fragment-background);color:var(--fragment-foreground);
  padding:10px;border-radius:6px;font-family:var(--font-family-monospace);font-size:13px;
  line-height:1.4;overflow-x:auto;">TEST: 'Check that expectation can be met'
  <span class="ansi-green">TEST: 'Check that expectation can be met' - [PASSED]</span>
  TEST: 'Check invalid detection'
  <span class="ansi-yellow">  [!!]: EMPTY TEST CASE</span>
  TEST: 'Check that expectation fails'
  <span class="ansi-red">  [X] [expect.cpp:15] : ** FAILURE ** : Expression: 1 == 2 evaluates to false.</span>
  <span class="ansi-bold">--------------------------------------------------------------------------------
  Results: 3 tests <span class="ansi-green">- 1/3 (33.33%) success </span><span class="ansi-red">- 1/3 (33.33%) failure </span><span class="ansi-yellow">- 1/3 (33.33%) invalid </span></span></pre>
  @endhtmlonly

  ## %tts::tap_sink

  [TAP](https://testanything.org/) (Test Anything Protocol) is a simple, language-agnostic text
  format for reporting test results, understood by many CI dashboards and test harnesses.
  `tts::tap_sink` listens to `test_finished()` and accumulates one `ok N - name` /
  `not ok N - name` line per @ref TTS_CASE, then `dump()` streams them out preceded by a leading
  `1..N` plan line.

  @code{cpp}
  tts::tap_sink tap;
  tts::output().sink(tap);

  // ... run the test suite ...

  tap.dump(); // stream the TAP-formatted report to stdout
  @endcode

  Or, without a custom driver: `./my_test --sink=tap` - `dump()` then happens automatically once
  the run finishes.

  Given a suite with one passing and one failing case, `tap.dump()` prints:

  @code{sh}
  1..2
  ok 1 - Check that expectation can be met
  not ok 2 - Check that expectation fails
  @endcode

  ## %tts::diagnostics_sink

  Wraps a target sink, forwarding every message unchanged, and additionally prints one
  `path:line: error: message` / `path:line: fatal error: message` line per failing/fatal
  assertion, so editors/IDEs with a GCC/Clang-style problem matcher (VS Code's C/C++ extension
  provides `$gcc`, vim has quickfix, ...) can jump straight to it. `assertion_failed()` fires
  before its corresponding
  text, so the diagnostic line prints first; it still fires under `-q`, when that raw line is
  itself suppressed.

  @code{cpp}
  tts::diagnostics_sink diagnostics;
  tts::output().sink(diagnostics);

  // ... run the test suite ...
  @endcode

  Or, without a custom driver: `./my_test --sink=diagnostics`.

  A failing @ref TTS_CASE then prints the diagnostic line, immediately followed by its usual one:

  @code{sh}
  expect.cpp:15: error: Expression: 1 == 2 evaluates to false.
    [X] [expect.cpp:15] : ** FAILURE ** : Expression: 1 == 2 evaluates to false.
  @endcode

  # Structured Format Sinks

  Unlike the simple sinks above, this one assembles its output into a single well-formed
  document with its own schema instead of printing independent lines - so it only ever produces
  output once the whole run has finished, via `dump()` or `finish()`, never incrementally.

  ## %tts::json_sink

  Accumulates one JSON object per @ref TTS_CASE from `test_finished()` and `assertion_failed()`,
  plus a `summary` counting passed/failed/invalid cases - counted directly as cases finish, not
  from the suite's raw assertion totals, so the number of entries in `tests` always matches
  `summary.total`. No JSON library involved: escaping is hand-rolled in `tts::_::json_escape()`,
  in keeping with @ref compile-time.

  ### Schema

  The document is a single object with two fields:

  Field      | Type            | Description
  ---------- | --------------- | -----------------------------------------------------------------
  `tests`    | array of object | One entry per @ref TTS_CASE that finished running, in run order.
  `summary`  | object          | Aggregate counts over every entry in `tests`.

  Each entry in `tests`:

  Field          | Type            | Description
  -------------- | --------------- | -------------------------------------------------------------
  `name`         | string          | The case's ID, exactly as given to @ref TTS_CASE.
  `status`       | string          | One of `"passed"`, `"failed"`, `"invalid"` (registered no assertion at all).
  `duration_ns`  | integer         | Wall-clock time the case took to run, in nanoseconds.
  `failures`     | array of object | One entry per failing/fatal assertion. Empty unless `status` is `"failed"`.

  Each entry in a `failures` array:

  Field       | Type    | Description
  ----------- | ------- | ------------------------------------------------------------------------
  `location`  | string  | `path:line` of the assertion, as reported in the human-readable output.
  `message`   | string  | The assertion's failure message.
  `fatal`     | boolean | `true` if this came from @ref TTS_FATAL (which then aborts the whole suite - see `summary` note below).

  `summary`:

  Field          | Type    | Description
  -------------- | ------- | -------------------------------------------------------------------
  `total`        | integer | Number of entries in `tests` (`passed` + `failed` + `invalid`).
  `passed`       | integer | Number of cases with `status: "passed"`.
  `failed`       | integer | Number of cases with `status: "failed"`.
  `invalid`      | integer | Number of cases with `status: "invalid"`.
  `duration_ns`  | integer | Sum of every case's `duration_ns`.

  @note A @ref TTS_FATAL assertion aborts the whole suite immediately - the case it happened in
  never finishes running, so it never reaches `test_finished()` and does **not** appear in
  `tests` at all, not even as a `"failed"` entry. `summary.total` then undercounts the suite's
  actual registered case count. This mirrors how @ref tts::tap_sink and the plain text output
  behave for the same reason: the driver loop's per-case bookkeeping only runs for a case that
  returns normally.

  @code{cpp}
  tts::json_sink json;
  tts::output().sink(json);

  // ... run the test suite ...

  json.dump(); // stream the JSON report to stdout
  @endcode

  Or, without a custom driver: `./my_test --sink=json`.

  Given a suite with one passing and one failing case, `json.dump()` prints (reformatted here
  for readability - the actual output is a single line):

  @code{json}
  {
    "tests": [
      {"name": "Check that expectation can be met", "status": "passed", "duration_ns": 996, "failures": []},
      {"name": "Check that expectation fails", "status": "failed", "duration_ns": 8145,
       "failures": [{"location": "expect.cpp:15", "message": "Expression: 1 == 2 evaluates to false.", "fatal": false}]}
    ],
    "summary": {"total": 2, "passed": 1, "failed": 1, "invalid": 0, "duration_ns": 9141}
  }
  @endcode
**/
//==================================================================================================
