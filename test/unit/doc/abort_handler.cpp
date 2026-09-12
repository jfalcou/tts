//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <iostream>

//! [snippet]
TTS_CASE("Choosing what a crashed or stuck run does before exiting")
{
  auto previous = tts::set_abort_handler(
  [](int reason) { std::cout << "tearing down after signal " << reason << "\n"; });

  TTS_EXPECT_NOT(previous);
};
//! [snippet]
