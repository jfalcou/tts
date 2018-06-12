//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//===================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Check that forced pass passes" )
{
//! [pass]
  TTS_PASS("Forced pass");
//! [pass]

//! [info]
  TTS_INFO("This is an info");
//! [info]

//! [warn]
  TTS_WARNING("This is a warning");
//! [warn]

//! [error]
  TTS_ERROR("This is an error");
//! [error]
}

