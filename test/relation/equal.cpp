 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of equality tests" )
{
  TTS_EQUAL(45,45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_EQUAL(a,b);
}

constexpr int f() { return 45; }

TTS_CASE( "Check correctness of constexpr equality tests" )
{
  TTS_CONSTEXPR_EQUAL(45,f());
}

#include "foo.hpp"

TTS_CASE( "Check correctness of equality tests on custom types" )
{
  foo::bar x{0.3f},y{0.3f};

  TTS_EQUAL(x,y);
}
