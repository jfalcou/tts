//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//===================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check that forced pass passes" )
{
  TTS_PASS("Forced pass");
}
