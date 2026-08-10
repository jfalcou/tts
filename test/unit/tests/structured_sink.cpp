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

    void test_finished(tts::text const& name, bool passed, bool invalid) override
    {
      log += tts::text {
      "FINISHED name=%s passed=%d invalid=%d\n", name.data(), passed ? 1 : 0, invalid ? 1 : 0};
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
  }

  bool        ok  = true;
  char const* log = sink.log.data();

  ok =
  ok && (strstr(log, "STARTED name=Passing case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok &&
  (strstr(log, "FINISHED name=Passing case for structured hooks passed=1 invalid=0\n") // NOSONAR
   != nullptr);

  ok =
  ok && (strstr(log, "STARTED name=Failing case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok && (strstr(log, "FAILED fatal=0 message=Expression: 1 == 2 evaluates to false.\n") // NOSONAR
         != nullptr);
  ok =
  ok &&
  (strstr(log, "FINISHED name=Failing case for structured hooks passed=0 invalid=0\n") // NOSONAR
   != nullptr);

  ok =
  ok && (strstr(log, "STARTED name=Invalid case for structured hooks\n") != nullptr); // NOSONAR
  ok =
  ok &&
  (strstr(log, "FINISHED name=Invalid case for structured hooks passed=0 invalid=1\n") // NOSONAR
   != nullptr);

  return ok ? 0 : 1;
}
