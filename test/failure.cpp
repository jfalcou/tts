//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_USE_CUSTOM_DRIVER
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>
#include <tts/tests/exceptions.hpp>
#include <tts/tests/precision.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/types.hpp>

//! [fail]
TTS_CASE( "Check that forced failure fails" )
{
  TTS_FAIL("Forced fail");
}
//! [fail]

TTS_CASE( "Check that forced broken expectation fails" )
{
  TTS_EXPECT(false == true);
  TTS_EXPECT_NOT(true == true);
}

void foo(bool x)  { if(x) throw 0; }

TTS_CASE( "Check that forced broken exceptions tests fails" )
{
  TTS_THROW(foo(false),int);
  TTS_NO_THROW(foo(true));
}

TTS_CASE( "Check that forced broken precision tests fails" )
{
  TTS_RELATIVE_EQUAL(1,2,0);
  TTS_ULP_EQUAL(1., 2., 0.5 );
}

TTS_CASE( "Check that forced broken relation tests fails" )
{
  TTS_EQUAL( 1, 0 );
  TTS_NOT_EQUAL( 1, 1 );
  TTS_LESS(1,0);
  TTS_GREATER(0,1);
  TTS_LESS_EQUAL(1,0);
  TTS_GREATER_EQUAL(0,1);
}

struct meta { template<typename T> struct apply { using type = T&; }; };

TTS_CASE( "Check that forced broken types tests fails" )
{
  TTS_TYPE_IS( int, float );
  TTS_EXPR_IS( 1.f , void**   );
}

int main(int argc, const char** argv)
{
  ::tts::env runtime(argc,argv,std::cout);
  return ::tts::run( runtime, ::tts::detail::suite, 15, 0 );
}
