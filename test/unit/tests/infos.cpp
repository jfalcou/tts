//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main
#include <tts/tts.hpp>

TTS_CASE("Check that forced failure fails")
{
  TTS_FAIL("Forced fail");
};

TTS_CASE("Check detection of invalid test")
{};
TTS_CASE("Check detection of more invalid tests")
{};

int main(int argc, char const** argv)
{
  fail_main(argc, argv);
  return ::tts::report(1, 2);
}
