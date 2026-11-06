//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
// Must not compile: a template case run in a named context declares its type through
// TTS_CONTEXT, and this body does not.
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE_TPL(TTS_CONSTEXPR("A template case without TTS_CONTEXT"), int, float)
<typename T>(tts::type<T>)
{
  TTS_EQUAL(T(1), T(1));
};
