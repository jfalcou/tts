//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION seed_reporting_main
#include <tts/tts.hpp>
#include <string_view>

TTS_CASE("Dummy test so the driver has something to run")
{
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  tts::gathering_sink gs;
  tts::output().sink(gs);

  seed_reporting_main(argc, argv);

  tts::output().sink(tts::output_handler::default_sink());

  tts::text        needle {"Random seed: %d", tts::random_seed()};
  std::string_view captured {gs.content().data()};

  TTS_EXPECT(captured.find(needle.data()) != std::string_view::npos);

  return tts::report(0, 0);
}
