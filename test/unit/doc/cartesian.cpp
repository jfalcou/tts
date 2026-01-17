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

TTS_CASE_TPL("Check types over cartesian product of types lists",
             tts::cartesian_product<tts::types<double, float>,
                                    tts::types<char, short>>)<typename T, typename U>(
    tts::type<tts::types<T, U>>)
{
  TTS_GREATER_EQUAL(sizeof(T), 4UL);
  TTS_LESS_EQUAL(sizeof(U), 2UL);
};
//! [snippet]
