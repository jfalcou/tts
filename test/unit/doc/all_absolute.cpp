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

TTS_CASE("Absolute distance over sequences")
{
  std::vector v {1.f, 2.f, 3.f, -1.f};
  std::list   w {1.f, 7.f, 3.f, -5.f};

  TTS_ALL_ABSOLUTE_EQUAL(v, w, 5);
};
//! [snippet]
