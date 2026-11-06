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

constexpr int square(int x)
{
  return x * x;
}

// The body runs as ordinary code, then as one constant expression
TTS_CASE(TTS_CONSTEXPR("Check square in both contexts"))
{
  int values[ 3 ] = {1, 2, 3};
  int sum         = 0;
  for(auto v: values)
    sum += square(v);

  TTS_EQUAL(sum, 14);
  TTS_EQUAL(square(-3), 9);
};
//! [snippet]
