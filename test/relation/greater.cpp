 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of greater-than comparison tests" )
{
  TTS_GREATER(69., 42ULL);

  std::vector<float> a{13,5}, b{2,4,6};
  TTS_GREATER(a,b);
}

constexpr int f() { return 9; }

TTS_CASE( "Check correctness of constexpr greater-than comparison tests" )
{
  TTS_CONSTEXPR_GREATER(17,f());
}

#include "foo.hpp"

TTS_CASE( "Check correctness of greater-than comparison tests on custom types" )
{
  foo::bar x{0.7f},y{0.1f};

  TTS_GREATER(x,y);
}
