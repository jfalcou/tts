//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

//! [snippet]
TTS_CASE("Choosing what a crashed or stuck run does before exiting")
{
  auto previous =
  tts::set_abort_handler([](int reason) { std::printf("tearing down after signal %d\n", reason); });

  TTS_EXPECT_NOT(previous);
};
//! [snippet]
