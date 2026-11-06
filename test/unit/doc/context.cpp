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

// A template case run in both contexts declares its type through TTS_CONTEXT
TTS_CASE_TPL(TTS_CONSTEXPR("Check the sum of two units for each type"), int, float, double)
<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(T(1) + T(1), T(2));
};

// A template case run in constant evaluation alone
TTS_CASE_TPL(TTS_CONSTEVAL("Check the product of two units for each type"), int, float)
<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(T(2) * T(2), T(4));
};
//! [snippet]
