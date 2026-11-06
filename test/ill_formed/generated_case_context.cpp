//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
// Must not compile: a generated case runs at run time alone, and a wrapped title on it is refused
// by a static assertion.
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE_WITH(TTS_CONSTEXPR("A context on a generated case"), (int, float), tts::value {37})
<typename V>(V v)
{
  TTS_EQUAL(v, V {37});
};
