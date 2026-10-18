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

// A type of four scalars
using pack4 = std::array<float, 4>;

pack4 twice(pack4 const& x)
{
  return {2 * x[ 0 ], 2 * x[ 1 ], 2 * x[ 2 ], 2 * x[ 3 ]};
}

namespace tts
{
  // One pack4 per four consecutive scalars
  template<> struct adapter<pack4>
  {
    using element_type                = float;
    static constexpr std::size_t size = 4;

    static pack4                 load(float const* src)
    {
      return {src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ]};
    }

    static void store(pack4 const& v, float* dst)
    {
      for(std::size_t i = 0; i < 4; ++i)
        dst[ i ] = v[ i ];
    }
  };
}

TTS_CASE("Range check of a function on packs against its scalar reference")
{
  auto twice_scalar = [](float x) { return 2 * x; };
  TTS_ULP_RANGE_CHECK(tts::between(-100.f, 100.f), float, pack4, twice_scalar, twice, 0.);
};
//! [snippet]
