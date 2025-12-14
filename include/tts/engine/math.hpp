//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once
#include <tts/tools/bitcast.hpp>

namespace tts::_
{
  #if defined(__FAST_MATH__)
    constexpr auto isnan(auto)            { return false; };
    constexpr auto isinf(auto)            { return false; };
    constexpr auto isunordered(auto,auto) { return false; };
  #else
    constexpr auto isnan(auto x)              { return x != x;                    };
    constexpr auto isinf(auto x)              { return !isnan(x) && isnan(x - x); };
    constexpr auto isunordered(auto x,auto y) { return  isnan(x) || isnan(y);     };
  #endif

    constexpr auto min(auto x, auto y) { return x<y ?  x : y;  };
    constexpr auto max(auto x, auto y) { return x<y ?  y : x;  };
    constexpr auto abs(auto x)         { return x<0 ? -x : x;  };
    constexpr bool signbit(auto x)     { return (as_int(x) >> (sizeof(x)*8-1)) != 0; };
}