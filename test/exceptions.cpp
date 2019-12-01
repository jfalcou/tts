//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/exceptions.hpp>

struct some_exception_type : std::exception {};

//! [throw]
void foo() { throw some_exception_type(); }

TTS_CASE( "Check that we can capture thrown exceptions" )
{
  TTS_THROW( foo(), some_exception_type );
}
//! [throw]

//! [no_throw]
void bar() noexcept { }

TTS_CASE( "Check that nothrow function are detected as such" )
{
  TTS_NO_THROW( bar() );
}
//! [no_throw]
