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

constexpr int twice(int x)
{
  return 2 * x;
}

// The constant context first, then the runtime one
TTS_CASE_IN("Check twice in a chosen order of contexts", tts::constant, tts::runtime)
{
  TTS_EQUAL(twice(21), 42);
};

// A tag wrapper on the title
TTS_CASE_IN(TTS_XFAIL("Check a wrong expectation in constant evaluation"), tts::constant)
{
  TTS_EQUAL(twice(21), 41);
};

// A template case, its types in parentheses, in constant evaluation alone
TTS_CASE_TPL_IN("Check twice for each type",
                (int, long),
                tts::constant)<typename T>(TTS_CONTEXT(tts::type<T>))
{
  TTS_EQUAL(twice(T(21)), T(42));
};
//! [snippet]
