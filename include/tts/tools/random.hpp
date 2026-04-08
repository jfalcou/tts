//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/math.hpp>
#include <cstdint>
#include <limits>
#include <concepts>
#include <algorithm>

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-random Utilities
    @{
  **/
  //====================================================================================================================

  namespace _
  {
    // SplitMix64 PRNG State
    inline thread_local std::uint64_t prng_state = 0x853c49e6748fea9bULL;

    // SplitMix64 generator: extremely fast, statistically robust,  reproducible
    inline std::uint64_t next_random()
    {
      std::uint64_t z = (prng_state += 0x9e3779b97f4a7c15ULL);
      z               = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
      z               = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
      return z ^ (z >> 31);
    }

    // Unbiased Integer Roll
    // Uses Rejection Sampling to fix modulo bias and extend range for full 64-bit capability
    template<std::integral T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);

      using U   = std::make_unsigned_t<T>;
      auto diff = static_cast<U>(static_cast<U>(N) - static_cast<U>(M));

      // If the range spans the absolute maximum of the data type (e.g., 0 to UINT64_MAX)
      if(diff == std::numeric_limits<U>::max())
      {
        return static_cast<T>(next_random());
      }

      auto          range       = static_cast<U>(diff + 1);
      std::uint64_t r_max       = std::numeric_limits<std::uint64_t>::max();
      std::uint64_t bucket_size = r_max / range;
      std::uint64_t limit       = bucket_size * range;

      std::uint64_t r;
      do
      {
        r = next_random();
      } while(r >= limit);

      return static_cast<T>(M + static_cast<T>(r / bucket_size));
    }

    // High-Resolution Float Roll
    // Uses full 64-bit precision to avoid quantization errors (step artifacts)
    template<std::floating_point T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);

      // Normalize exactly to [0, 1] based on full 64-bit precision
      T uniform_01 =
      static_cast<T>(next_random()) / static_cast<T>(std::numeric_limits<std::uint64_t>::max());

      // Use proportional interpolation to avoid N - M overflow
      // (e.g., when N = numeric_limits::max and M = numeric_limits::lowest)
      return (T(1.0) - uniform_01) * M + uniform_01 * N;
    }

    template<std::integral T> T roll_random(T mini, T maxi)
    {
      return _::roll(mini, maxi);
    }

    // Floating point complex logic (Logarithmic distribution)
    template<std::floating_point T> T roll_random(T mini, T maxi)
    {
      constexpr T smvlp     = std::numeric_limits<T>::min();
      constexpr T valmax    = std::numeric_limits<T>::max();
      constexpr T eps       = std::numeric_limits<T>::epsilon();
      constexpr T quiet_nan = std::numeric_limits<T>::quiet_NaN();

      // Sanitize 0
      if(mini == 0) mini = smvlp;
      if(maxi == 0) maxi = -smvlp;

      // Fix Infinite inputs (Prevent NaN propagation)
      if(_::is_inf(mini) && mini < 0) mini = -valmax;
      if(_::is_inf(maxi) && maxi > 0) maxi = valmax;

      // Handle Zero-Crossing Ranges [-A, +B]
      if(mini < 0 && maxi > 0)
      {
        T abs_min = _::abs(mini);
        T abs_max = _::abs(maxi);

        // Weight probability by magnitude
        T    total_mag     = abs_min + abs_max;
        bool pick_positive = _::roll(T(0.0), total_mag) < abs_max;

        if(pick_positive) return roll_random(smvlp, maxi);
        else return -roll_random(smvlp, abs_min);
      }

      T value = {};

      // Handle Single-Sided Ranges
      if(mini > 0)
      {
        if(mini < 1 && maxi > 1) mini = max(T(1) / _::sqrt(maxi), mini);
        mini      = (maxi == 1) ? eps : mini;

        T log_min = _::log10(mini);
        T log_max = _::log10(maxi);

        // Roll in log-space
        T log_val = _::roll(log_min, log_max);
        value     = _::exp10(log_val);
      }
      else if(maxi < 0)
      {
        // Mirror logic for negatives
        if(mini < -1 && maxi > -1) maxi = min(T(1) / _::sqrt(-mini), maxi);
        maxi      = (mini == -1) ? -eps : maxi;

        T log_min = _::log10(-maxi);
        T log_max = _::log10(-mini);

        T log_val = _::roll(log_min, log_max);
        value     = -_::exp10(log_val);
      }
      else
      {
        return quiet_nan;
      }

      return value;
    }
  }

  //====================================================================================================================
  /**
    @public
    @brief Sets the seed for the internal PRNG to guarantee reproducibility.
    @ingroup tools-random

    @param seed The 64-bit seed value to initialize the generator state.
  **/
  //====================================================================================================================
  inline void set_random_seed(std::uint64_t seed)
  {
    _::prng_state = seed;
  }

  //====================================================================================================================
  /**
    @public
    @brief Generates a random value between two bounds.
    @ingroup tools-random

    This function generates a random value of type `T` between the specified `mini` and `maxi`
    bounds using an uniform distribution for integral types and a log-uniform distribution for
    floating-point types.

    @groupheader{Example}
    @snippet doc/random.cpp snippet

    @param mini Lower bound for the random value generation.
    @param maxi Upper bound for the random value generation.
    @return A random value of type `T` between `mini` and `maxi`.

    @see tts::randoms
  **/
  //====================================================================================================================
  template<typename T> T random_value(T mini, T maxi)
  {
    return _::roll_random(mini, maxi);
  }
}
