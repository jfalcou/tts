//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of inequality tests" )
{
  TTS_NOT_EQUAL(42,69.f);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_NOT_EQUAL(a,b);
};

constexpr int f() { return 45; }

TTS_CASE( "Check correctness of constexpr inequality tests" )
{
  TTS_CONSTEXPR_NOT_EQUAL(63,f());
};

#include "foo.hpp"

TTS_CASE( "Check correctness of inequality tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_NOT_EQUAL(x,y);
};
