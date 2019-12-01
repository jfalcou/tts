 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//! [less]
#include <tts/tts.hpp>
#include <tts/tests/relation.hpp>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_LESS(42., 69);

  std::vector<float> a{1,2,3}, b{12,3};
  TTS_LESS(a,b);
}
//! [less]

#include "foo.hpp"

TTS_CASE( "Check correctness of less-than comparison tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_LESS(x,y);
}
