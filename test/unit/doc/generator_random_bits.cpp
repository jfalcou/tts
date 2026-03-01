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

TTS_CASE_WITH("random bits generator example", tts::arithmetic_types, tts::random_bits {})
<typename T>(T)
{
  TTS_EXPECT(std::is_unsigned_v<T>);
};
