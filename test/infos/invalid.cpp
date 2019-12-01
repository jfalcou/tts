//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_USE_CUSTOM_DRIVER

//! [invalid]
#include <tts/tts.hpp>
#include <tts/tests/infos.hpp>

TTS_CASE( "Check forcing a test to be invalid" )
{
  TTS_INVALID("Forced invalid");
}
//! [invalid]

TTS_CASE( "Check detection of invalid test" )        { (void)runtime; }
TTS_CASE( "Check detection of more invalid tests" )  { (void)runtime; }

int main(int argc, char** argv)
{
  ::tts::env runtime(argc,argv,std::cout);
  return ::tts::run( runtime, ::tts::detail::suite, 0, 3 );
}
