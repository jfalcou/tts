//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

TTS_CASE("Equality for sequences")
{
  std::vector<float> vx{1,2,3,4,5};

  TTS_ALL_EQUAL(vx ,vx);
}
