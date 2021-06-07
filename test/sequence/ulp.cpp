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

TTS_CASE("ULP distance between sequences")
{
  auto eps = std::numeric_limits<float>::epsilon();

  std::vector<float> a{1.f,1.f+eps,1.f-eps,1.f+3*eps};
  std::vector<float> b{1.f,1.f,1.f,1.f};

  TTS_ALL_ULP_EQUAL(a, b , 1.5);
};
