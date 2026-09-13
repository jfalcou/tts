//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <csignal>

//! [snippet]
TTS_CASE("a case that runs to the end")
{
  TTS_EQUAL(1 + 1, 2);
};

#if !defined(__EMSCRIPTEN__) // no signal delivery there, so nothing would name the case below
// A crash ends the run, so the tag turns that end into the expected one and the exit code into 0.
TTS_CASE(TTS_XFAIL("a case that brings the run down"))
{
  // A raised signal keeps memcheck and the sanitizers silent: no invalid access happens.
  std::raise(SIGSEGV);
};
#endif

TTS_CASE("a case the run never reaches")
{
  TTS_EXPECT(true);
};
//! [snippet]
