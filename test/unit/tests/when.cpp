//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE( "Check test with sub-test" )
{
  TTS_WHEN("We start some sub-test")
  {
    int i = 99;

    TTS_AND_THEN("We increment a variable")
    {
      TTS_EQUAL(i,99);
      i++;
      TTS_EQUAL(i,100);
    }

    TTS_AND_THEN("We decrement a variable")
    {
      TTS_EQUAL(i,99);
      i--;
      TTS_EQUAL(i,98);
    }
  }
};
