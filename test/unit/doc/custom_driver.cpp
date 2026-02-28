//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION my_test_main
#include <tts/tts.hpp>
#include <iostream>

TTS_CASE("Some test")
{
// This test will be counted as invalid because it does not contain any assertion.
};

int main(int argc, char const** argv)
{
  std::cout << "Initializing tests...\n";
  ::tts::initialize(argc, argv);

  std::cout << "Running tests...\n";
  my_test_main(argc, argv);

  std::cout << "Finalizing tests...\n";
  return ::tts::report(0, 1);
}
//! [snippet]
