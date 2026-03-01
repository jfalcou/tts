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

TTS_CASE_WITH("Test tts::ramp generator",
              (float, double),
              tts::as_signed_integer {
              tts::randoms {-10, 10}
})
<typename T>(T value)
{
  TTS_EXPECT(std::is_integral_v<decltype(value)>);
  TTS_EXPECT(std::is_signed_v<decltype(value)>);
  TTS_LESS_EQUAL(value, T {10});
  TTS_GREATER_EQUAL(value, T {-10});
};
//! [snippet]
