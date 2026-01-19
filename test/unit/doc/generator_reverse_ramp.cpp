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

TTS_CASE_WITH("Test tts::reverse_ramp generator",
              (std::array<float, 10>, int, double),
              tts::reverse_ramp {65},
              tts::reverse_ramp {10, 2})<typename T>(T const& ramp1, T const& ramp2)
{
  if constexpr(std::is_arithmetic_v<T>)
  {
    TTS_EQUAL(ramp1, static_cast<T>(65));
    TTS_EQUAL(ramp2, static_cast<T>(10));
  }
  else
  {
    using value_type = typename T::value_type;
    for(std::size_t i = 0; i < ramp1.size(); ++i)
    {
      TTS_EQUAL(ramp1[ i ], static_cast<value_type>(65) - static_cast<value_type>(i));
    }
    for(std::size_t i = 0; i < ramp2.size(); ++i)
    {
      TTS_EQUAL(ramp2[ i ], static_cast<value_type>(10) - static_cast<value_type>(i * 2));
    }
  }
};
//! [snippet]
