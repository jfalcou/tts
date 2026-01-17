//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

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

TTS_CASE( "Check correctness of typed inequality tests" )
{
  TTS_TYPED_NOT_EQUAL(45.f,7.7f);

  std::vector<float>  a{1,2,3}, b{1,2};
  TTS_TYPED_NOT_EQUAL(a,b);
};

TTS_CASE( "Check correctness of constexpr typed inequality tests" )
{
  TTS_TYPED_CONSTEXPR_NOT_EQUAL(77,f());
};

#include "foo.hpp"

TTS_CASE( "Check correctness of inequality tests on custom types" )
{
  foo::bar x{0.3f},y{0.7f};

  TTS_NOT_EQUAL(x,y);
};
