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
#include <list>

TTS_CASE( "ULP distance over sequences")
{
  std::vector v{1.f       , 2.f, 3.f, -5.f};
  std::list   w{1.f+1e-7f , 2.f, 3.f, -5.f};

  TTS_ALL_ULP_EQUAL(v,w,0.5);
};
//! [snippet]