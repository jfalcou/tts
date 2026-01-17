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

TTS_CASE( "Check correctness of less-equal comparison tests" )
{
  TTS_LESS_EQUAL(42., 69);
  TTS_LESS_EQUAL(69., 69);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_LESS_EQUAL(a,b);
  TTS_LESS_EQUAL(a,a);

  // This test would cause the program to stop with a failure
  // TTS_LESS_EQUAL(10., 1., REQUIRED);
};
//! [snippet]