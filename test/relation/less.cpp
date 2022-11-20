 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of less-than comparison tests" )
{
  TTS_LESS(42., 69);

  std::vector<float> a{1,2,3}, b{12,3};
  TTS_LESS(a,b);
};

constexpr int f() { return 45; }

TTS_CASE( "Check correctness of constexpr less-than comparison tests" )
{
  TTS_CONSTEXPR_LESS(17,f());
};

#include "foo.hpp"

TTS_CASE( "Check correctness of less-than comparison tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_LESS(x,y);
};
