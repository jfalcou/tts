//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main
#include <tts/tts.hpp>

TTS_CASE( "Check that forced broken expectation fails" )
{
  TTS_EXPECT(false == true);
  TTS_EXPECT_NOT(true == true);
}

TTS_CASE( "Check that forced broken relation tests fails" )
{
  int x = 0;
  TTS_EQUAL( 1, x );
  TTS_NOT_EQUAL( x, x );
  TTS_LESS(1,x);
  TTS_GREATER(x,1);
  TTS_LESS_EQUAL(1,x);
  TTS_GREATER_EQUAL(x,1);
}

void foo(bool x)  { if(x) throw std::runtime_error{"THIS IS AN ERROR"}; }

TTS_CASE( "Check that forced broken exceptions tests fails" )
{
  TTS_THROW(foo(false), std::runtime_error);
  TTS_THROW(foo(true) , std::bad_alloc    );
  TTS_NO_THROW(foo(true));
}

TTS_CASE( "Check that forced broken precision tests fails" )
{
  double x = 1. + 1e-15;

  TTS_RELATIVE_EQUAL(1,2,0);
  TTS_ULP_EQUAL(1., x, 0.5 );
  TTS_IEEE_EQUAL(1., x );
  TTS_ABSOLUTE_EQUAL(x ,1., 1e-16 );
}

TTS_CASE( "Check that forced broken precision tests fails on array" )
{
  std::vector<float> a{1.f,1.f,1.f,1.f};
  std::vector<float> b{2.f,-1.f,1.f,5.f};

  TTS_ALL_EQUAL(a, b);
  TTS_ALL_ULP_EQUAL(a, b, 0.5);
  TTS_ALL_IEEE_EQUAL(a, b);
  TTS_ALL_RELATIVE_EQUAL(a, b, 5);
  TTS_ALL_ABSOLUTE_EQUAL(a, b, 1);
}

TTS_CASE( "Check that forced broken types tests fails" )
{
  TTS_TYPE_IS( int , float  );
  TTS_EXPR_IS( 1.f , void** );
}

int main(int argc, char const** argv)
{
  fail_main(argc, argv);
  return ::tts::report(22,0);
}
