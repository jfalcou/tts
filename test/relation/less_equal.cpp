 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//! [less-equal]
#include <tts/tts.hpp>
#include <tts/tests/relation.hpp>

TTS_CASE( "Check correctness of less-equal comparison tests" )
{
  TTS_LESS_EQUAL(42., 69);
  TTS_LESS_EQUAL(69., 69);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_LESS_EQUAL(a,b);
  TTS_LESS_EQUAL(a,a);
}
//! [less-equal]

#include "foo.hpp"

TTS_CASE( "Check correctness of less-equal comparison tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_LESS_EQUAL(x,y);
  TTS_LESS_EQUAL(x,x);
}
