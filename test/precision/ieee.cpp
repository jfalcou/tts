//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//====================================================================================================
#include <tts/tts.hpp>

TTS_CASE("IEEE equality")
{
  float x = std::numeric_limits<float>::quiet_NaN();
  TTS_IEEE_EQUAL(x,x);

  TTS_IEEE_EQUAL(1.f, 1.f);
  TTS_IEEE_EQUAL(2. , 2. );
  TTS_IEEE_EQUAL(65 , 'A');
};

#include "my_real.hpp"

TTS_CASE("IEEE equality of type with custom ulpdist")
{
  TTS_IEEE_EQUAL(n1::my_real{0.f}, n1::my_real{1.f});
};
