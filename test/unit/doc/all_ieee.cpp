//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE("IEEE distance over sequences")
{
  float       x = std::numeric_limits<float>::quiet_NaN();
  std::vector v {1.f, 2.f, 3.f, x};
  std::list   w {1.f, 2.f, 3.f, x};

  TTS_ALL_IEEE_EQUAL(v, w);
};
//! [snippet]
