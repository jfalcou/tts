//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/exceptions.hpp>

struct some_exception_type : std::exception {};

//! [throwing_foo]
void foo(bool x)
{
  if(x)
    throw some_exception_type();
}
//! [throwing_foo]

//! [throw]
TTS_CASE( "Check that we can capture thrown exceptions" )
{
  TTS_THROW( foo(true), some_exception_type );
}
//! [throw]

//! [no_throw]
TTS_CASE( "Check that nothrow function are detected as such" )
{
  TTS_NO_THROW( foo(false) );
}
//! [no_throw]
