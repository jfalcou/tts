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

TTS_CASE("Relative distance between sequences")
{
  std::vector<float> a{1.f,1.1f,0.9f,1.3f};
  std::vector<float> b{1.f,1.f,1.f,1.f};
  std::vector<float> s{1.1f};

  TTS_ALL_RELATIVE_EQUAL(a  , b   , 25);
  TTS_ALL_RELATIVE_EQUAL(s  , 1.f , 25);
  TTS_ALL_RELATIVE_EQUAL(1.f, s   , 25);
}
