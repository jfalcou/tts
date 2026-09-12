//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

//! [snippet]
TTS_CASE(TTS_TIMEOUT(5000, "a collective that answers in time"))
{
  TTS_EXPECT(true);
};

TTS_CASE(TTS_XFAIL(TTS_TIMEOUT(0, "known to sit there, and exempt from --timeout")))
{
  TTS_EXPECT(1 == 2);
};
//! [snippet]
