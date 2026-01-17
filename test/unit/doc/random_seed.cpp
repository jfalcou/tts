//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION random_main
#include <tts/tts.hpp>

TTS_CASE("Usage of tts::random_seed")
{
  TTS_EQUAL(tts::random_seed(), 1234);

  // Calling with a different seed does not change the current one
  tts::random_seed(999);
  TTS_EQUAL(tts::random_seed(), 1234);
};

int main(int argc, const char **argv)
{
  tts::random_seed(1234);

  printf("TTS - Random Seed : %d\n", tts::random_seed());

  random_main(argc, argv);
  return tts::report(0, 0);
}
//! [snippet]
