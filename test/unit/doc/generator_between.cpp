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

TTS_CASE_WITH("Test tts::between generator",
              (std::array<float, 10>, int, double),
              tts::between {-4, 4})<typename T>(T const &args)
{
  if constexpr(std::is_arithmetic_v<T>) { TTS_EQUAL(args, static_cast<T>(-4)); }
  else
  {
    using value_type = typename T::value_type;
    for(std::size_t i = 0; i < args.size(); ++i)
    {
      TTS_ULP_EQUAL(
          args[ i ],
          static_cast<value_type>(-4 + i * 8 / static_cast<value_type>((args.size() - 1))),
          4);
    }
  }
};
//! [snippet]
