//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

TTS_CASE("IEEE equality for sequences")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  std::vector<float> vx{x,x,x,x};

  TTS_ALL_IEEE_EQUAL(vx,vx);
};
