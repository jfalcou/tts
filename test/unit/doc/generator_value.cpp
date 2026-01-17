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

TTS_CASE_WITH("Test tts::value generator",
              (std::array<float, 10>, int, double),
              tts::value {13.37})<typename T>(T const &args)
{
  if constexpr(std::is_arithmetic_v<T>) { TTS_EQUAL(args, static_cast<T>(13.37)); }
  else
  {
    for(std::size_t i = 0; i < args.size(); ++i)
    {
      TTS_EQUAL(args[ i ], static_cast<typename T::value_type>(13.37));
    }
  }
};
//! [snippet]
