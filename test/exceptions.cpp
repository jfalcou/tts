//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/exceptions.hpp>

struct some_exception_type {};

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
