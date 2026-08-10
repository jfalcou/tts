//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION suite_aborted_main
#include <tts/tts.hpp>

TTS_CASE("Fatal case triggering an early abort")
{
  TTS_FATAL("boom");
};

namespace
{
  struct recording_sink : tts::output_sink
  {
    void write(tts::text const&) override
    {
      // Unused: this test only exercises the suite_aborted() hook.
    }

    void suite_aborted() override
    {
      aborted = true;
    }

    bool aborted = false;
  };
}

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  recording_sink sink;
  {
    tts::scoped_sink scope(sink);
    suite_aborted_main(argc, argv);
  }

  return sink.aborted ? 0 : 1;
}
