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

  All three draw from @ref tts::output_sink's structured hooks (`test_started()`,
  `assertion_failed()`, `test_finished()`, `suite_finished()`, `suite_metric()`,
  `suite_aborted()`) rather than by parsing the text `tts::stdout_sink` prints, so they stay
  correct regardless of `-v`/`-q`. None of them currently reflect @ref TTS_CASE_TPL's per-type
  breakdown or @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since those don't have their own
  hook.

  @note On a plain @ref TTS_MAIN binary (no custom driver), the `--sink=name` CLI flag installs
  any of the three below without writing any C++ at all - `--sink=colored`, `--sink=tap`, or
  `--sink=diagnostics`. It composes with `--capture=path`: `--sink=tap --capture=report.tap`
  writes TAP-formatted output to the file instead of stdout. Like `--shard`, it's a no-op for
  binaries using a @ref TTS_CUSTOM_DRIVER_FUNCTION, which already manages its own sink - see
  @ref cli for the full flag reference.

  # Simple Format Sinks

  Each of these produces plain, line-oriented text - as opposed to a hypothetical structured-
  format sink (JSON, JUnit XML, ...), which would need to assemble a single well-formed document
  with its own schema instead of just lines of text.

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

  @see tts::output_sink
  @see tts::output_handler
**/
//==================================================================================================
