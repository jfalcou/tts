//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <chrono>
#include <thread>

//! [snippet]
TTS_CASE(TTS_TIMEOUT(5000, "a collective that answers in time"))
{
  TTS_EXPECT(true);
};

TTS_CASE(TTS_XFAIL(TTS_TIMEOUT(0, "known to sit there, and exempt from --timeout")))
{
  TTS_EXPECT(1 == 2);
};

#if !defined(__EMSCRIPTEN__) // no process timer there, so nothing would end the case below
// The deadline ends the run, so the tag turns that end into the expected one and the exit code
// into 0.
TTS_CASE(TTS_XFAIL(TTS_TIMEOUT(250, "a case that outlives its own deadline")))
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
};
#endif
//! [snippet]
