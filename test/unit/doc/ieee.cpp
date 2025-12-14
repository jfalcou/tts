//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>

TTS_CASE( "ULP distance")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  TTS_IEEE_EQUAL(x,x);

  TTS_IEEE_EQUAL(1.f, 1.f);
  TTS_IEEE_EQUAL(2. , 2. );
  TTS_IEEE_EQUAL(65 , 'A');
};
//! [snippet]