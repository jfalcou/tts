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

TTS_CASE("Equality for sequences")
{
  std::vector<float> vx{1,2,3,4,5};
  std::vector<float> vy{6};

  TTS_ALL_EQUAL(vx ,vx   );
  TTS_ALL_EQUAL(vy , 6.f );
  TTS_ALL_EQUAL(6.f, vy  );
}
