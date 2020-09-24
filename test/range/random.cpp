//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/random_range.hpp>

float ok_x  (float x) { return x; }
float ajar_x(float x) { return x+1e-7f; }

TTS_CASE( "Test some range on float" )
{
  tts::rng_producer<float>  p(1.f,2.f);
  TTS_ULP_RANGE_CHECK(p,ok_x,ajar_x,8192);
}

int absc  (int x) { return x; }
int absc_c(int x) { return x % 2 ? x*2 : x; }

TTS_CASE( "Test some range on int" )
{
  tts::rng_producer<int>  p(-100,+100);
  TTS_ULP_RANGE_CHECK(p,absc,absc_c,128);
}