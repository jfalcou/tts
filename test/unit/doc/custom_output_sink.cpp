//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION custom_sink_main
#include <tts/tts.hpp>

// A power-user output_sink prefixing every message TTS writes before forwarding it to stdout.
struct prefixed_sink : tts::output_sink
{
  int  messages = 0;

  void write(tts::text const& t) override
  {
    messages++;
    if(t != "\n") fputs("[MY-TTS] ", stdout);
    fputs(t.data(), stdout);
  }
};

TTS_CASE("Some test running through our custom output_sink")
{
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  prefixed_sink sink;
  tts::output().sink(sink); // Install our custom sink...

  custom_sink_main(argc, argv); // ...run the test suite through it...

  tts::output().sink(tts::output_handler::default_sink()); // ...then restore the default one.

  printf("Our custom output_sink received %d message(s)\n", sink.messages);

  return tts::report(0, 0);
}
//! [snippet]
