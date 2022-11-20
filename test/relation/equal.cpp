 //==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Check correctness of equality tests" )
{
  TTS_EQUAL(45,45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_EQUAL(a,b);
};

constexpr int f() { return 45; }

TTS_CASE( "Check correctness of constexpr equality tests" )
{
  constexpr int value = 45;
  TTS_CONSTEXPR_EQUAL(value,f()) << "ERROR with: " << value;
};

TTS_CASE( "Check correctness of typed equality tests" )
{
  TTS_TYPED_EQUAL(45.f,45.f);

  std::vector<float> a{1,2,3}, b{1,2,3};
  TTS_TYPED_EQUAL(a,b);
};

TTS_CASE( "Check correctness of constexpr typed equality tests" )
{
  TTS_TYPED_CONSTEXPR_EQUAL(45,f());
};

#include "foo.hpp"

TTS_CASE( "Check correctness of equality tests on custom types" )
{
  foo::bar x{0.3f},y{0.3f};

  TTS_EQUAL(x,y);
};

TTS_CASE( "Check correctness of typed equality tests on custom types" )
{
  foo::bar x{0.3f},y{0.3f};

  TTS_TYPED_EQUAL(x,y);
};
