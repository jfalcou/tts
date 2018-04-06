//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>

// ![all_ulp]
TTS_CASE("ULP distance between containers")
{
  auto eps = std::numeric_limits<float>::epsilon();

  std::vector<float> a{1.f,1.f+eps,1.f-eps,1.f+3*eps};
  std::vector<float> b{1.f,1.f,1.f,1.f};
  std::vector<float> s{1.f+eps};

  TTS_ALL_ULP_EQUAL(a  , b   , 1.5);
  TTS_ALL_ULP_EQUAL(s  , 1.f , 1.5);
  TTS_ALL_ULP_EQUAL(1.f, s   , 1.5);
}
// ![all_ulp]

TTS_CASE("IEEE equality for containers")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  std::vector<float> vx{x,x,x,x};

  TTS_ALL_IEEE_EQUAL(vx,vx);
}
