//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

struct some_exception_type
{
};

void foo()
{
  throw some_exception_type();
}

TTS_CASE("Check that we can capture thrown exceptions")
{
  TTS_THROW(foo(), some_exception_type);
};

void bar() noexcept
{
  // This function is marked as noexcept, so it should not throw any exceptions.
  // If it does, the test framework should detect this and report it as a failure.
  // In this case, we are not throwing anything, so the test should pass.
}

TTS_CASE("Check that nothrow function are detected as such")
{
  TTS_NO_THROW(bar());
};
