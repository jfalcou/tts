//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of greater-than comparison tests" )
{
  TTS_GREATER(69., 42ULL);

  std::vector<float> a{13,5}, b{2,4,6};
  TTS_GREATER(a,b);

  // This test will cause the program to stop with a failure
  // TTS_GREATER(1., 10., REQUIRED);
};
//! [snippet]