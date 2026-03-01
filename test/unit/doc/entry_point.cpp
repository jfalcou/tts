//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION custom_entry_point
#include <tts/tts.hpp>
#include <iostream>

TTS_CASE("Tautological test")
{
  TTS_EXPECT_NOT(false == true);
};

int main(int argc, char const** argv)
{
  std::cout << "Welcome to a special test driver !\n";
  custom_entry_point(argc, argv);
  return tts::report(0, 0);
}
//! [snippet]
