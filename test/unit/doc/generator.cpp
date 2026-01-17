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

struct flip_values
{
  constexpr flip_values(int p = 1)
      : period_(p)
      , value_(-1)
  {
  }

  template<typename T> auto operator()(tts::type<T>, auto i, auto)
  {
    if((i % period_) == 0) value_ = -value_;
    return static_cast<T>(value_);
  }

  int period_, value_;
};

TTS_CASE_WITH("Test custom generator", (std::array<float, 10>, std::array<int, 4>), flip_values {3})
(auto const &args)
{
  for(std::size_t i = 0; i < args.size(); ++i)
  {
    TTS_EQUAL(args[ i ], ((i % 3 == 0) ? 1.0f : -1.0f));
  }
};
//! [snippet]
