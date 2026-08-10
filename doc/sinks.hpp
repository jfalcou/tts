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

  `tts::colorized_sink` and `tts::diagnostics_sink` work directly on the human-readable text
  `tts::stdout_sink` already prints, recognizing TTS's own wording ("[PASSED]", "** FAILURE **",
  ...). `tts::tap_sink` instead draws from @ref output_sink's structured `test_finished()` hook,
  so it stays correct regardless of `-v`/`-q`. None of them currently reflect
  @ref TTS_CASE_TPL's per-type breakdown or @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since
  those don't have their own hook or a distinct line of their own to key off of.

  Sink                       | Purpose
  -------------------------- | -----------------------------------------------------------------
  `tts::colorized_sink`      | Wraps another sink, coloring pass/fail/fatal lines with ANSI escapes.
  `tts::tap_sink`            | Gathers the run, then renders it as TAP (Test Anything Protocol).
  `tts::diagnostics_sink`    | Rewrites failure/fatal lines as compiler-style diagnostics.

  # tts::colorized_sink

  Wraps a target sink (`tts::output_handler::default_sink()` by default) and colors messages that
  look like a pass confirmation (green), a failure/fatal/abort (red), an invalid test (yellow), or
  the final `Results: ...` summary (green if everything passed, red otherwise) - forwarding
  everything else unchanged. Opt-in: not every terminal or CI log renders ANSI escapes usefully,
  and on Windows it additionally depends on the host console having Virtual Terminal Processing
  enabled (most modern terminals already do).

  @code{cpp}
  tts::colorized_sink colorized;
  tts::output().sink(colorized);

  // ... run the test suite ...
  @endcode

  A failing run then looks like the usual TTS output, except `[PASSED]` lines are wrapped in
  `\033[32m...\033[0m` (green), failure/fatal lines in `\033[31m...\033[0m` (red), and so on -
  exactly what a terminal needs to render them in color.

  # tts::tap_sink

  Listens to `test_finished()` and accumulates one `ok N - name` / `not ok N - name` line per
  @ref TTS_CASE, then `dump()` streams them out preceded by a leading `1..N` plan line.

  @code{cpp}
  tts::tap_sink tap;
  tts::output().sink(tap);

  // ... run the test suite ...

  tap.dump(); // stream the TAP-formatted report to stdout
  @endcode

  Given a suite with one passing and one failing case, `tap.dump()` prints:

  @code{sh}
  1..2
  ok 1 - Check that expectation can be met
  not ok 2 - Check that expectation fails
  @endcode

  # tts::diagnostics_sink

  Wraps a target sink and rewrites lines that look like a @ref TTS_CASE failure or fatal error -
  which already embed a `[file:line]` location - into `file:line: error: message` /
  `file:line: fatal error: message`, so editors/IDEs with a GCC/Clang-style problem matcher
  (VSCode's `$gcc`, vim's quickfix, ...) can jump straight to the failing assertion. Every other
  message passes through unchanged.

  @code{cpp}
  tts::diagnostics_sink diagnostics;
  tts::output().sink(diagnostics);

  // ... run the test suite ...
  @endcode

  A failure that would normally print as:

  @code{sh}
    [X] [expect.cpp:15] : ** FAILURE ** : Expression: 1 == 2 evaluates to false.
  @endcode

  instead prints as:

  @code{sh}
  expect.cpp:15: error: Expression: 1 == 2 evaluates to false.
  @endcode

  @see tts::output_sink
  @see tts::output_handler
**/
//==================================================================================================
