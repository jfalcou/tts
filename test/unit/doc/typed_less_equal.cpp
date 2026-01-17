//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main
//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <vector>

TTS_CASE("Check correctness of type checked less-equal comparison tests")
{
  TTS_TYPED_LESS_EQUAL(42.f, 69LL);
  TTS_TYPED_LESS_EQUAL(42.f, 69.f);
  TTS_TYPED_LESS_EQUAL(69, 69);

  std::vector<float> a {1, 2, 3}, b {79, 11};
  TTS_TYPED_LESS_EQUAL(a, b);
  TTS_TYPED_LESS_EQUAL(a, a);
};
//! [snippet]

int main(int argc, char const **argv)
{
  ::tts::initialize(argc, argv);
  fail_main(argc, argv);
  return ::tts::report(1, 0);
}
