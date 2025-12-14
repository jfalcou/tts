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

TTS_CASE( "Check correctness of type checked greater-than comparison tests" )
{
  TTS_TYPED_GREATER(69., 42ULL);
  TTS_TYPED_GREATER(69., 42.);

  std::vector<float> a{13,5}, b{2,4,6};
  TTS_TYPED_GREATER(a,b);
};
//! [snippet]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc,argv);
  fail_main(argc, argv);
  return ::tts::report(1,0);
}
