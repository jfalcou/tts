//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/sequence.hpp>

TTS_CASE("Absolute distance between sequences")
{
  std::vector<float> a{1.f,1.1f,0.9f,1.3f};
  std::vector<float> b{1.f,1.f,1.f,1.f};
  std::vector<float> s{1.1f};

  TTS_ALL_ABSOLUTE_EQUAL(a  , b   , .4);
  TTS_ALL_ABSOLUTE_EQUAL(s  , 1.f , .15);
  TTS_ALL_ABSOLUTE_EQUAL(1.f, s   , .15);
}
