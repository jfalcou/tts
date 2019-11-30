//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//===================================================================================================
//! [pass]
#include <tts/tts.hpp>
#include <tts/tests/infos.hpp>

TTS_CASE( "Check that forced pass passes" )
{
  TTS_PASS("Forced pass");
}
//! [pass]
