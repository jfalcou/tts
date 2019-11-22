//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>
#include <vector>

TTS_CASE("IEEE equality for sequences")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  std::vector<float> vx{x,x,x,x};

  TTS_ALL_IEEE_EQUAL(vx,vx);
}
