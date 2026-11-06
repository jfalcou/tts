//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
// Must not compile: the constant context of a TTS_CONSTEXPR case calls a function that is not
// constexpr.
#define TTS_MAIN
#include <tts/tts.hpp>

int runtime_only(int v)
{
  return v;
}

TTS_CASE(TTS_CONSTEXPR("A runtime call in a constexpr case"))
{
  TTS_EQUAL(runtime_only(1), 1);
};
