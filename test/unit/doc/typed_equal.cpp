//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of type checked equality tests" )
{
  TTS_TYPED_EQUAL(45, 45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_TYPED_EQUAL(a,b);
};
//! [snippet]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc,argv);
  fail_main(argc, argv);
  return ::tts::report(1,0);
}
