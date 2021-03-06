//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main
#include <tts/tts.hpp>

TTS_CASE( "Check that forced failure fails" )
{
  TTS_FAIL("Forced fail");
};

int main(int argc, char const** argv)
{
  fail_main(argc, argv);
  return ::tts::report(1,0);
}
