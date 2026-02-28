//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

void bar()
{
  // This function is marked as noexcept, so it should not throw any exceptions.
  // If it does, the test framework should detect this and report it as a failure.
  // In this case, we are not throwing anything, so the test should pass.
}

TTS_CASE("Check that nothrow function are detected as such")
{
  TTS_NO_THROW(bar());
};
//! [snippet]
