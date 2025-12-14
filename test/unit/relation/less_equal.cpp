 //==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

TTS_CASE( "Check correctness of less-equal comparison tests" )
{
  TTS_LESS_EQUAL(42., 69);
  TTS_LESS_EQUAL(69., 69);

  std::vector<float> a{1,2,3}, b{7,9};
  TTS_LESS_EQUAL(a,b);
  TTS_LESS_EQUAL(a,a);
};

constexpr int f() { return 45; }

TTS_CASE( "Check correctness of constexpr less or equal than comparison tests" )
{
  TTS_CONSTEXPR_LESS_EQUAL(17,f());
  TTS_CONSTEXPR_LESS_EQUAL(45,f());
};

#include "foo.hpp"

TTS_CASE( "Check correctness of less-equal comparison tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_LESS_EQUAL(x,y);
  TTS_LESS_EQUAL(x,x);
};
