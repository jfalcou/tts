//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#ifndef RANGE_HPP_INCLUDED
#define RANGE_HPP_INCLUDED

#include <tts/tts.hpp>
#include <array>

// std::array<X, 4> stands for a type of four values of X
namespace tts
{
  template<typename X> struct adapter<std::array<X, 4>>
  {
    using element_type                = X;
    static constexpr std::size_t size = 4;

    static std::array<X, 4>      load(X const* src) noexcept
    {
      return {src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ]};
    }

    static void store(std::array<X, 4> const& v, X* dst) noexcept
    {
      dst[ 0 ] = v[ 0 ];
      dst[ 1 ] = v[ 1 ];
      dst[ 2 ] = v[ 2 ];
      dst[ 3 ] = v[ 3 ];
    }
  };
}

// A ramp has odd values at any block size, which several cases need.
inline auto ramp = [](auto, auto i, auto) { return static_cast<int>(i); };

#endif
