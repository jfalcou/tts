 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//! [greater-equal]
#include <tts/tts.hpp>
#include <tts/tests/relation.hpp>

TTS_CASE( "Check correctness of greater-equal comparison tests" )
{
  TTS_GREATER_EQUAL(69LL, 42.f);
  TTS_GREATER_EQUAL(69., 69);

  std::vector<float> a{12,3}, b{7,9,11};
  TTS_GREATER_EQUAL(a,b);
  TTS_GREATER_EQUAL(a,a);
}
//! [greater-equal]

#include "foo.hpp"

TTS_CASE( "Check correctness of greater-equal comparison tests on custom types" )
{
  foo::bar x{0.7f},y{0.5f};

  TTS_GREATER_EQUAL(x,y);
  TTS_GREATER_EQUAL(x,x);
}
