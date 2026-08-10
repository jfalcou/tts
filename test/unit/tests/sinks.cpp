//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION sinks_test_main
#include <tts/tts.hpp>

TTS_CASE("Passing case for sink tests")
{
  TTS_EXPECT(1 == 1);
};

TTS_CASE("Failing case for sink tests")
{
  TTS_EXPECT(1 == 2);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool ok = true;

  // colorized_sink wraps a target sink and colors PASSED/FAILURE lines, including the
  // multi-fragment "Results: ..." summary, without breaking any content through.
  {
    tts::gathering_sink target;
    tts::colorized_sink colorized {target};
    {
      tts::scoped_sink scope(colorized);
      sinks_test_main(argc, argv);
      tts::report(1, 0); // emits the "Results: ..." summary this block also checks below
    }

    char const* content = target.content().data();
    ok                  = ok && (strstr(content, "\033[32m") != nullptr);   // NOSONAR - green
    ok                  = ok && (strstr(content, "\033[31m") != nullptr);   // NOSONAR - red
    ok                  = ok && (strstr(content, "\033[0m") != nullptr);    // NOSONAR - reset
    ok = ok && (strstr(content, "Passing case for sink tests") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "Failing case for sink tests") != nullptr); // NOSONAR

    // The Results: summary itself (not just some earlier line) must be red, since this suite has
    // a failure - it stays active color across the separator right above it too.
    ok = ok && (strstr(content, "\033[31mResults:") != nullptr); // NOSONAR
  }

  // tap_sink renders "1..N" / "ok" / "not ok" lines from the structured test_finished() events.
  {
    tts::tap_sink tap;
    {
      tts::scoped_sink scope(tap);
      sinks_test_main(argc, argv);
    }

    tts::gathering_sink target;
    tap.dump(target);
    char const* content = target.content().data();

    ok                  = ok && (strstr(content, "1..2") != nullptr);                  // NOSONAR
    ok = ok && (strstr(content, "ok 1 - Passing case for sink tests") != nullptr);     // NOSONAR
    ok = ok && (strstr(content, "not ok 2 - Failing case for sink tests") != nullptr); // NOSONAR
  }

  // diagnostics_sink adds a "file:line: error: message" line per failing/fatal assertion.
  {
    tts::gathering_sink   target;
    tts::diagnostics_sink diagnostics {target};
    {
      tts::scoped_sink scope(diagnostics);
      sinks_test_main(argc, argv);
    }

    char const* content = target.content().data();
    ok                  = ok && (strstr(content, "sinks.cpp:") != nullptr); // NOSONAR
    ok                  = ok &&
         (strstr(content, ": error: Expression: 1 == 2 evaluates to false.") != nullptr); // NOSONAR
  }

  return ok ? 0 : 1;
}
