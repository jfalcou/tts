//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main

//! [snippet1]
#define TTS_MAIN                   // No need for main()
#include <tts/tts.hpp>

TTS_CASE("Check that forced pass passes") { TTS_PASS("Forced success!!"); };
//! [snippet1]

#undef TTS_MAIN

//! [snippet2]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

TTS_CASE("Check that forced failure fails")
{
  TTS_FAIL("Forced failure!!");
  TTS_PASS("This will pass though");
};
//! [snippet2]

#undef TTS_MAIN

//! [snippet3]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

TTS_CASE("Check that forced major failure fails")
{
  TTS_PASS("This will pass");
  TTS_FATAL("Forced major failure!!");
  TTS_PASS("This won't pass");
};
//! [snippet3]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);
  fail_main(argc, argv);
  return ::tts::report(2, 0);
}
