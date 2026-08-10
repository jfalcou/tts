#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  output-sinks Built-in Output Sinks

  @tableofcontents

  # Overview

  TTS has an extensible @ref tts::output_sink customization point (see @ref tools-config and the
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

  On a plain @ref TTS_MAIN binary (no custom driver), the `--sink=name` CLI flag installs any of
  the three below without writing any C++ at all - see @ref cli.

  Sink                       | Purpose
  -------------------------- | -----------------------------------------------------------------
  `tts::colorized_sink`      | Wraps another sink, coloring pass/fail/fatal lines with ANSI escapes.
  `tts::tap_sink`            | Gathers the run, then renders it as TAP (Test Anything Protocol).
  `tts::diagnostics_sink`    | Adds compiler-style diagnostics for failure/fatal assertions.

  # tts::colorized_sink

  Wraps a target sink (`tts::output_handler::default_sink()` by default) and colors a pass
  confirmation (green), a failure/fatal/abort (red), an invalid test (yellow) - forwarding
  everything else unchanged. The separator and the `Results: ...` prefix are bold and color-
  neutral; only the `- N/M (P%) <label>` segment for each outcome category (success, failure,
  invalid) is colored, in bold green/red/yellow respectively - so a run with both failures and
  invalids shows each count in its own color rather than the whole line in one. A color stays
  active across consecutive lines/segments until the next hook changes it. Opt-in: not every
  terminal or CI log renders ANSI escapes usefully, and on Windows it additionally depends on the
  host console having Virtual Terminal Processing enabled (most modern terminals already do).

  @code{cpp}
  tts::colorized_sink colorized;
  tts::output().sink(colorized);

  // ... run the test suite ...
  @endcode

  Or, without a custom driver: `./my_test --sink=colored`.

  A failing run then looks like the usual TTS output, except `[PASSED]` lines are wrapped in
  `\033[32m...\033[0m` (green), failure/fatal lines in `\033[31m...\033[0m` (red), the `Results:`
  line and separator in `\033[1m...\033[0m` (bold), and each of its outcome segments in
  `\033[1;32m`/`\033[1;31m`/`\033[1;33m` (bold green/red/yellow) - exactly what a terminal needs
  to render them in color.

  # tts::tap_sink

  Listens to `test_finished()` and accumulates one `ok N - name` / `not ok N - name` line per
  @ref TTS_CASE, then `dump()` streams them out preceded by a leading `1..N` plan line.

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

  # tts::diagnostics_sink

  Wraps a target sink, forwarding every message unchanged, and additionally prints one
  `file:line: error: message` / `file:line: fatal error: message` line per failing/fatal
  assertion, so editors/IDEs with a GCC/Clang-style problem matcher (VSCode's `$gcc`, vim's
  quickfix, ...) can jump straight to it. `assertion_failed()` fires before its corresponding
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
