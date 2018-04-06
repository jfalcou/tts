//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
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

