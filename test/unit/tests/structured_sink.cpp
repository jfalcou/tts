//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION structured_sink_main
#include <tts/tts.hpp>

TTS_CASE("Passing case for structured hooks")
{
  TTS_EXPECT(1 == 1);
};

TTS_CASE("Failing case for structured hooks")
{
  TTS_EXPECT(1 == 2);
};

TTS_CASE("Invalid case for structured hooks")
{
// Expect to be invalid because of missing assertion
};

namespace
{
  // Records every structured hook call as one tagged line per event, so the test below can
  // check for exact, unambiguous lines instead of loose substring matches.
  struct recording_sink : tts::output_sink
  {
    void write(tts::text const&) override
    {
      // Unused: this test only exercises the structured hooks, not the text stream.
    }

    void test_started(tts::text const& name) override
    {
      log += tts::text {"STARTED name=%s\n", name.data()};
    }

    void assertion_failed(tts::text const& location, tts::text const& message, bool fatal) override
    {
      log += tts::text {"FAILED fatal=%d message=%s\n", fatal ? 1 : 0, message.data()};
      (void)location;
    }

    void test_finished(tts::text const&   name,
                       bool               passed,
                       bool               invalid,
                       unsigned long long duration_ns) override
    {
      // Sane upper bound (under 10s), not an exact value - actual timing is inherently
      // machine-dependent and would make this test flaky.
      bool sane  = duration_ns < 10'000'000'000ULL;
      log       += tts::text {"FINISHED name=%s passed=%d invalid=%d duration_sane=%d\n",
                        name.data(),
                        passed ? 1 : 0,
                        invalid ? 1 : 0,
                        sane ? 1 : 0};
    }

    void suite_finished(unsigned long long fail_count, unsigned long long invalid_count) override
    {
      log += tts::text {"SUITE_FINISHED fails=%llu invalids=%llu\n", fail_count, invalid_count};
    }

    tts::text log;
  };
}

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  recording_sink sink;
  {
    tts::scoped_sink scope(sink);
    structured_sink_main(argc, argv);
    tts::report(1, 1); // triggers suite_finished() with this suite's actual fail/invalid counts
  }

  bool        ok  = true;
  char const* log = sink.log.data();

  ok =
  ok && (strstr(log, "STARTED name=Passing case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok &&
  (strstr(log, // NOSONAR
          "FINISHED name=Passing case for structured hooks passed=1 invalid=0 duration_sane=1\n") !=
   nullptr);

  ok =
  ok && (strstr(log, "STARTED name=Failing case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok && (strstr(log, "FAILED fatal=0 message=Expression: 1 == 2 evaluates to false.\n") // NOSONAR
         != nullptr);
  ok =
  ok &&
  (strstr(log, // NOSONAR
          "FINISHED name=Failing case for structured hooks passed=0 invalid=0 duration_sane=1\n") !=
   nullptr);

  ok =
  ok && (strstr(log, "STARTED name=Invalid case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok &&
  (strstr(log, // NOSONAR
          "FINISHED name=Invalid case for structured hooks passed=0 invalid=1 duration_sane=1\n") !=
   nullptr);

  ok = ok && (strstr(log, "SUITE_FINISHED fails=1 invalids=1\n") != nullptr); // NOSONAR

  return ok ? 0 : 1;
}
