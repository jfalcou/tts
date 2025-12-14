//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>
#include <cmath>

TTS_CASE("Equality over sequence")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.f,2.f,3.f,-5.f};
  std::list   z{1.f,2.f,0.f,-5.f};

  TTS_ALL_ABSOLUTE_EQUAL(v,v,0);
  TTS_ALL_ABSOLUTE_EQUAL(v,w,0);
  TTS_ALL_ABSOLUTE_EQUAL(v,z,3);
};

TTS_CASE("Exact equality over sequence")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.f,2.f,3.f,-5.f};

  TTS_ALL_EQUAL(v,v);
  TTS_ALL_EQUAL(v,w);
};

TTS_CASE("Relative equality over sequence")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.f,2.f,3.f,-5.f};
  std::list   z{1.f,2.f,3.1f,-5.f};

  TTS_ALL_RELATIVE_EQUAL(v,v,0);
  TTS_ALL_RELATIVE_EQUAL(v,w,0);
  TTS_ALL_RELATIVE_EQUAL(v,z,3.33);
};

TTS_CASE("ULP equality over sequence")
{
  std::vector v{1.f       , 2.f, 3.f      , -5.f};
  std::list   w{1.f+1e-7f , 2.f, 3.f      , -5.f};
  std::list   z{1.f       , 2.f, 3.f+3e-6f, -5.f};

  TTS_ALL_ULP_EQUAL(v,v,0);
  TTS_ALL_ULP_EQUAL(v,w,0.5);
  TTS_ALL_ULP_EQUAL(v,z,10);
};

TTS_CASE("Strict IEEE equality over sequence")
{
  std::vector v{1.f, 2.f, 3.f, 4.f};
  std::list   w{1.f, std::sqrt(4.f), std::sqrt(9.f), std::sqrt(16.f)};

  TTS_ALL_IEEE_EQUAL(v,v);
  TTS_ALL_IEEE_EQUAL(v,w);
};
