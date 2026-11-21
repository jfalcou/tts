//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check as_int")
{
  TTS_EQUAL(tts::_::as_int(-0.f), 0x80000000U);
  TTS_EQUAL(tts::_::as_int(1.f), 0x3F800000U);
  TTS_EQUAL(tts::_::as_int(0.f), 0x00000000U);
  TTS_EQUAL(tts::_::as_int(-0.), 0x8000000000000000UL);
  TTS_EQUAL(tts::_::as_int(1.), 0x3FF0000000000000UL);
  TTS_EQUAL(tts::_::as_int(0.), 0x0000000000000000UL);
};
