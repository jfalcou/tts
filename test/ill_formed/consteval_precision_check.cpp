//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
// Must not compile: a precision check runs at run time alone, and a TTS_CONSTEVAL case has no
// runtime context.
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE(TTS_CONSTEVAL("A precision check in a consteval case"))
{
  TTS_ULP_EQUAL(1.f, 1.f, 0.5);
};
