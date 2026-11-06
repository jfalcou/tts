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

consteval int table_size(int n)
{
  return n * n + 1;
}

// The body runs as one constant expression, the only context a consteval function allows
TTS_CASE(TTS_CONSTEVAL("Check table_size in constant evaluation"))
{
  TTS_EQUAL(table_size(0), 1);
  TTS_EQUAL(table_size(4), 17);
};
//! [snippet]
