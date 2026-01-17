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

TTS_CASE_WITH("Test tts::randoms generator",
              (std::array<float, 10>, std::array<int, 7>, double),
              tts::randoms {-4, 4})<typename T>(T const &args)
{
  if constexpr(std::is_arithmetic_v<T>)
  {
    TTS_GREATER_EQUAL(args, static_cast<T>(-4));
    TTS_LESS_EQUAL(args, static_cast<T>(4));
  }
  else
  {
    using value_type = typename T::value_type;
    for(std::size_t i = 0; i < args.size(); ++i)
    {
      TTS_GREATER_EQUAL(args[ i ], static_cast<value_type>(-4));
      TTS_LESS_EQUAL(args[ i ], static_cast<value_type>(4));
    }
  }
};
//! [snippet]
