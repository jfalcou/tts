//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#define TTS_USE_CUSTOM_DRIVER
#include <tts/tts.hpp>

TTS_CASE( "Check detection of invalid test" )        { (void)runtime; }
TTS_CASE( "Check detection of more invalid tests" )  { (void)runtime; }

int main(int argc, const char** argv)
{
  ::tts::env runtime(argc,argv,std::cout);
  return ::tts::run( runtime, ::tts::detail::suite, 0, 2 );
}
