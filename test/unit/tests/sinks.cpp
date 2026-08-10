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

namespace
{
  // colorized_sink wraps a target sink and colors PASSED/FAILURE lines, including the
  // multi-fragment "Results: ..." summary, without breaking any content through.
  bool check_colorized(int argc, char const** argv)
  {
    tts::gathering_sink target;
    tts::colorized_sink colorized {target};
    {
      tts::scoped_sink scope(colorized);
      sinks_test_main(argc, argv);
      tts::report(1, 0); // emits the "Results: ..." summary this block also checks below
    }

    char const* content = target.content().data();
    bool        ok      = true;
    ok                  = ok && (strstr(content, "\033[32m") != nullptr);   // NOSONAR - green
    ok                  = ok && (strstr(content, "\033[31m") != nullptr);   // NOSONAR - red
    ok                  = ok && (strstr(content, "\033[0m") != nullptr);    // NOSONAR - reset
    ok = ok && (strstr(content, "Passing case for sink tests") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "Failing case for sink tests") != nullptr); // NOSONAR

    // The separator and the "Results:" prefix are bold neutral, not tied to pass/fail; only each
    // score segment (success/failure/invalid) is colored, per its own category.
    ok = ok && (strstr(content, "\033[1m--------") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "\033[1mResults:") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "\033[1;32m- 1/2") != nullptr); // NOSONAR - success
    ok = ok && (strstr(content, "\033[1;31m- 1/2") != nullptr); // NOSONAR - failure
    return ok;
  }

  // tap_sink renders "1..N" / "ok" / "not ok" lines from the structured test_finished() events.
  bool check_tap(int argc, char const** argv)
  {
    tts::tap_sink tap;
    {
      tts::scoped_sink scope(tap);
      sinks_test_main(argc, argv);
    }

    tts::gathering_sink target;
    tap.dump(target);
    char const* content = target.content().data();

    bool        ok      = true;
    ok                  = ok && (strstr(content, "1..2") != nullptr);                  // NOSONAR
    ok = ok && (strstr(content, "ok 1 - Passing case for sink tests") != nullptr);     // NOSONAR
    ok = ok && (strstr(content, "not ok 2 - Failing case for sink tests") != nullptr); // NOSONAR
    return ok;
  }

  // diagnostics_sink adds a "file:line: error: message" line per failing/fatal assertion.
  bool check_diagnostics(int argc, char const** argv)
  {
    tts::gathering_sink   target;
    tts::diagnostics_sink diagnostics {target};
    {
      tts::scoped_sink scope(diagnostics);
      sinks_test_main(argc, argv);
    }

    char const* content = target.content().data();
    bool        ok      = true;
    ok                  = ok && (strstr(content, "sinks.cpp:") != nullptr); // NOSONAR
    ok                  = ok &&
         (strstr(content, ": error: Expression: 1 == 2 evaluates to false.") != nullptr); // NOSONAR
    return ok;
  }

  // json_sink renders one "tests" entry per TTS_CASE plus a case-level "summary", built from the
  // structured hooks - the counts must match what actually ran, not the suite's raw assertion
  // totals.
  bool check_json(int argc, char const** argv)
  {
    tts::json_sink json;
    {
      tts::scoped_sink scope(json);
      sinks_test_main(argc, argv);
    }

    tts::gathering_sink target;
    json.dump(target);
    char const* content = target.content().data();

    bool        ok      = true;
    ok = ok && (strstr(content, "\"name\":\"Passing case for sink tests\"") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "\"status\":\"passed\"") != nullptr);                    // NOSONAR
    ok = ok && (strstr(content, "\"name\":\"Failing case for sink tests\"") != nullptr); // NOSONAR
    ok = ok && (strstr(content, "\"status\":\"failed\"") != nullptr);                    // NOSONAR
    ok =
    ok && (strstr(content, "\"location\":{\"file\":\"sinks.cpp\",\"line\":") != nullptr); // NOSONAR
    ok =
    ok && (strstr(content, "\"message\":\"Expression: 1 == 2 evaluates to false.\"") != // NOSONAR
           nullptr);
    char const* summary = R"("total":2,"passed":1,"failed":1,"invalid":0)";
    ok                  = ok && (strstr(content, summary) != nullptr); // NOSONAR
    return ok;
  }

  // junit_sink renders one <testcase> per TTS_CASE inside a single <testsuite>, with a
  // <failure> child gathering every failing assertion for a failed case.
  bool check_junit(int argc, char const** argv)
  {
    tts::junit_sink junit;
    {
      tts::scoped_sink scope(junit);
      sinks_test_main(argc, argv);
    }

    tts::gathering_sink target;
    junit.dump(target);
    char const* content      = target.content().data();

    char const* suite_header = R"(<testsuite name="TTS" tests="2" failures="1" errors="0")"
                               R"( skipped="0")";
    bool        ok           = true;
    ok                       = ok && (strstr(content, suite_header) != nullptr); // NOSONAR
    ok                       = ok &&
         (strstr(content, R"(<testcase name="Passing case for sink tests")") != nullptr); // NOSONAR
    ok = ok &&
         (strstr(content, R"(<testcase name="Failing case for sink tests")") != nullptr); // NOSONAR
    char const* failure_marker = R"(<failure message="Expression: 1 == 2 evaluates to false.")"
                                 R"(>sinks.cpp:)";
    ok                         = ok && (strstr(content, failure_marker) != nullptr); // NOSONAR
    return ok;
  }
}

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool ok = true;
  ok      = ok && check_colorized(argc, argv);
  ok      = ok && check_tap(argc, argv);
  ok      = ok && check_diagnostics(argc, argv);
  ok      = ok && check_json(argc, argv);
  ok      = ok && check_junit(argc, argv);

  return ok ? 0 : 1;
}
