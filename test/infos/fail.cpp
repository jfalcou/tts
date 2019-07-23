//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_USE_CUSTOM_DRIVER
#include <tts/tts.hpp>
#include <tts/tests/infos.hpp>

TTS_CASE( "Check that forced failure fails" )
{
  TTS_FAIL("Forced fail");
}

int main(int argc, char** argv)
{
  ::tts::env runtime(argc,argv,std::cout);
  return ::tts::run( runtime, ::tts::detail::suite, 1, 0 );
}
