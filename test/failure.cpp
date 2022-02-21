//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main
#include <tts/tts.hpp>
#include <vector>
#include <list>

TTS_CASE( "Invalid test" )                                                    {};
TTS_CASE_TPL( "EmpInvalidty template tests", ::tts::arithmetic_types )(auto)  {};

TTS_CASE( "Check that forced broken expectation fails" )
{
  TTS_EXPECT(false == true);
  TTS_EXPECT_NOT(true == true);
};

TTS_CASE( "Check that forced broken relation tests fails" )
{
  int x = 0;
  TTS_EQUAL( 1.0123456789, x );
  TTS_NOT_EQUAL( x, x );
  TTS_LESS(1.95f,x);
  TTS_GREATER(x,1.789);
  TTS_LESS_EQUAL(2,x);
  TTS_GREATER_EQUAL(x,3.5f);
};

void foo(bool x)  { if(x) throw std::runtime_error{"THIS IS AN ERROR"}; }

TTS_CASE( "Check that forced broken exceptions tests fails" )
{
  TTS_THROW(foo(false), std::runtime_error);
  TTS_THROW(foo(true) , std::bad_alloc    );
  TTS_NO_THROW(foo(true));
};

TTS_CASE( "Check that forced broken precision tests fails" )
{
  double x = 1. + 1e-15;

  TTS_RELATIVE_EQUAL(1,2,0);
  TTS_ULP_EQUAL(1., x, 0.5 );
  TTS_IEEE_EQUAL(1., x );
  TTS_ABSOLUTE_EQUAL(x ,1., 1e-16 );
};

TTS_CASE("Check broken sequence comparisons")
{
  std::vector v{1.f,2.f,3.f,-5.f};
  std::list   w{1.000001f,2.5f,3.f,-5.f};

  TTS_ALL_ABSOLUTE_EQUAL(v,w,0);
  TTS_ALL_RELATIVE_EQUAL(v,w,10);
  TTS_ALL_ULP_EQUAL(v,w,0.5);
  TTS_ALL_IEEE_EQUAL(v,w);
};

int main(int argc, char const** argv)
{
  fail_main(argc, argv);
  return ::tts::report(19,2);
}
