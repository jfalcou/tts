//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION invalid_main
#include <tts/tts.hpp>

TTS_CASE( "Check forcing a test to be invalid" )      { TTS_INVALID("Forced invalid"); };
TTS_CASE( "Check detection of invalid test" )         {};
TTS_CASE( "Check detection of more invalid tests" )   {};

int main(int argc, char const** argv)
{
  invalid_main(argc, argv);
  return ::tts::report(0,3);
}
