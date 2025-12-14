//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

struct some_exception_type {};

void foo() { throw some_exception_type(); }

TTS_CASE( "Check that we can capture thrown exceptions" )
{
  TTS_THROW( foo(), some_exception_type );
};

void bar() noexcept { }

TTS_CASE( "Check that nothrow function are detected as such" )
{
  TTS_NO_THROW( bar() );
};
