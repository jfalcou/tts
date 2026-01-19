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
#include <iostream>

TTS_CASE("Check test with sub-test") {TTS_WHEN("We start some sub-test") {int i = 99;

TTS_AND_THEN("We increment a variable")
{
  std::cout << ">> Sub-test 1: i = " << i << "\n";
  TTS_EQUAL(i, 99);
  i++;
  TTS_EQUAL(i, 100);
}

TTS_AND_THEN("We decrement a variable")
{
  std::cout << ">> Sub-test 2: i = " << i << "\n";
  TTS_EQUAL(i, 99);
  i--;
  TTS_EQUAL(i, 98);
}
}
}
;
//! [snippet]
