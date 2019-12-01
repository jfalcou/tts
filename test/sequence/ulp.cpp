//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//! [all-ulp-equal]
#include <tts/tts.hpp>
#include <tts/tests/sequence.hpp>
#include <vector>

TTS_CASE("ULP distance between sequences")
{
  auto eps = std::numeric_limits<float>::epsilon();

  std::vector<float> a{1.f,1.f+eps,1.f-eps,1.f+3*eps};
  std::vector<float> b{1.f,1.f,1.f,1.f};
  std::vector<float> s{1.f+eps};

  TTS_ALL_ULP_EQUAL(a  , b   , 1.5);
  TTS_ALL_ULP_EQUAL(s  , 1.f , 1.5);
  TTS_ALL_ULP_EQUAL(1.f, s   , 1.5);
}
//! [all-ulp-equal]
