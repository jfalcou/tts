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
#include <array>

TTS_CASE_WITH("Test tts::ramp generator",
              (std::array<float, 10>, int, double),
              tts::ramp {65},
              tts::ramp {1, 2})<typename T>(T const &ramp1, T const &ramp2)
{
  if constexpr(std::is_arithmetic_v<T>)
  {
    TTS_EQUAL(ramp1, static_cast<T>(65));
    TTS_EQUAL(ramp2, static_cast<T>(1));
  }
  else
  {
    for(std::size_t i = 0; i < ramp1.size(); ++i)
    {
      TTS_EQUAL(ramp1[ i ], static_cast<typename T::value_type>(65 + i));
    }
    for(std::size_t i = 0; i < ramp2.size(); ++i)
    {
      TTS_EQUAL(ramp2[ i ], static_cast<typename T::value_type>(1 + i * 2));
    }
  }
};
//! [snippet]
