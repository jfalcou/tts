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
TTS_CASE("Check correctness of bitwise inequality tests")
{
  TTS_BIT_NOT_EQUAL(1.f, 0x3F80F700);
  TTS_BIT_NOT_EQUAL(2., 0x3FF0000000000000ULL);

  // This test would cause the program to stop with a failure.
  // TTS_BIT_NOT_EQUAL(21, 21, REQUIRED);
};
//! [snippet]
