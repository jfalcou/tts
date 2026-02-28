//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once
#include <tts/tools/bitcast.hpp>
#include <bit>

#if !defined(__GNUC__) && !defined(__clang__)
#define TTS_USE_STDMATH
#include <math.h>
#endif

namespace tts::_
{
#if defined(__FAST_MATH__)
  constexpr auto is_nan(auto)
  {
    return false;
  };
  constexpr auto is_inf(auto)
  {
    return false;
  };
  constexpr auto is_unordered(auto, auto)
  {
    return false;
  };
#else
  constexpr auto is_nan(auto x)
  {
    return x != x;
  };
  constexpr auto is_inf(auto x)
  {
    return !is_nan(x) && is_nan(x - x);
  };
  constexpr auto is_unordered(auto x, auto y)
  {
    return is_nan(x) || is_nan(y);
  };
#endif

  constexpr auto min(auto x, auto y)
  {
    return x < y ? x : y;
  };
  constexpr auto max(auto x, auto y)
  {
    return x < y ? y : x;
  };
  constexpr auto abs(auto x)
  {
    return x < 0 ? -x : x;
  };
  constexpr bool signbit(auto x)
  {
    return (as_int(x) >> (sizeof(x) * 8 - 1)) != 0;
  };

  //====================================================================================================================
  // Math wrappers
  //====================================================================================================================
#if !defined(TTS_USE_STDMATH)
  // POW
  inline double pow(double x, double y)
  {
    return __builtin_pow(x, y);
  }
  inline float pow(float x, float y)
  {
    return __builtin_powf(x, y);
  }
  inline long double pow(long double x, long double y)
  {
    return __builtin_powl(x, y);
  }

  // SQRT
  inline double sqrt(double x)
  {
    return __builtin_sqrt(x);
  }
  inline float sqrt(float x)
  {
    return __builtin_sqrtf(x);
  }
  inline long double sqrt(long double x)
  {
    return __builtin_sqrtl(x);
  }

  // LOG10
  inline double log10(double x)
  {
    return __builtin_log10(x);
  }
  inline float log10(float x)
  {
    return __builtin_log10f(x);
  }
  inline long double log10(long double x)
  {
    return __builtin_log10l(x);
  }

  // CEIL
  inline double ceil(double x)
  {
    return __builtin_ceil(x);
  }
  inline float ceil(float x)
  {
    return __builtin_ceilf(x);
  }
  inline long double ceil(long double x)
  {
    return __builtin_ceill(x);
  }

  // LOG2 (Floating point)
  inline double log2(double x)
  {
    return __builtin_log2(x);
  }
  inline float log2(float x)
  {
    return __builtin_log2f(x);
  }
  inline long double log2(long double x)
  {
    return __builtin_log2l(x);
  }
#else
  inline double pow(double x, double y)
  {
    return ::pow(x, y);
  }
  inline float pow(float x, float y)
  {
    return ::powf(x, y);
  }
  inline long double pow(long double x, long double y)
  {
    return ::powl(x, y);
  }

  inline double sqrt(double x)
  {
    return ::sqrt(x);
  }
  inline float sqrt(float x)
  {
    return ::sqrtf(x);
  }
  inline long double sqrt(long double x)
  {
    return ::sqrtl(x);
  }

  inline double log10(double x)
  {
    return ::log10(x);
  }
  inline float log10(float x)
  {
    return ::log10f(x);
  }
  inline long double log10(long double x)
  {
    return ::log10l(x);
  }

  inline double ceil(double x)
  {
    return ::ceil(x);
  }
  inline float ceil(float x)
  {
    return ::ceilf(x);
  }
  inline long double ceil(long double x)
  {
    return ::ceill(x);
  }

  inline double log2(double x)
  {
    return ::log2(x);
  }
  inline float log2(float x)
  {
    return ::log2f(x);
  }
  inline long double log2(long double x)
  {
    return ::log2l(x);
  }
#endif

  constexpr inline std::size_t log2(std::size_t n)
  {
    return n ? std::bit_width(n) - 1 : 0;
  }
  template<typename T> T exp10(T a)
  {
    return tts::_::pow(T(10), a);
  }
}
