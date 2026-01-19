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

template<typename T> struct small_type : std::bool_constant<(sizeof(T) < 4)>
{
};

TTS_CASE_TPL("Check types over filtered types list",
             tts::filter<small_type, tts::arithmetic_types>::types_list)<typename T>(tts::type<T>)
{
  TTS_LESS(sizeof(T), 4UL);
};
//! [snippet]
