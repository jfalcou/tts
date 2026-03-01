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
    // RAND_MAX can be very small on some paltrform (like 32767) so we need to ensure enough bits
    struct rand_result
    {
      unsigned int val, max;
    };

    inline rand_result rand30()
    {
      if constexpr(RAND_MAX >= 2147483647)
      {
        return {static_cast<unsigned int>(std::rand()), static_cast<unsigned int>(RAND_MAX)};
      }
      else
      {
        constexpr unsigned int SHIFT_MAX =
        (static_cast<unsigned int>(RAND_MAX) << 15) | static_cast<unsigned int>(RAND_MAX);
        unsigned int r =
        (static_cast<unsigned int>(std::rand()) << 15) | static_cast<unsigned int>(std::rand());
        return {r, SHIFT_MAX};
      }
    }

    // Unbiased Integer Roll
    // Uses Rejection Sampling to fix modulo bias and extend range
    template<std::integral T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);

      using U = std::make_unsigned_t<T>;
      U range = static_cast<U>(N - M) + 1;

      if(range <= static_cast<U>(RAND_MAX))
      {
        unsigned int r_max       = static_cast<unsigned int>(RAND_MAX);
        unsigned int bucket_size = r_max / static_cast<unsigned int>(range);
        unsigned int limit       = bucket_size * static_cast<unsigned int>(range);

        unsigned int r;
        do
        {
          r = static_cast<unsigned int>(std::rand());
        } while(r >= limit);

        return M + static_cast<T>(r / bucket_size);
      }

      auto [ r_raw, r_max ] = rand30();

      // Fallback for huge ranges exceeding our 30-bit generator
      if(range > r_max)
      {
        return M + static_cast<T>(r_raw % range);
      }

      // Rejection Sampling: discard the "tail" of the random range
      U            bucket_size = r_max / range;
      U            limit       = bucket_size * range;

      unsigned int r           = r_raw;
      while(r >= limit)
      {
        r = rand30().val;
      }

      return M + static_cast<T>(r / bucket_size);
    }

    // High-Resolution Float Roll
    // Uses 30-bit precision to avoid quantization errors (step artifacts)
    template<std::floating_point T> T roll(T M, T N)
    {
      if(M == N) return M;
      if(M > N) std::swap(M, N);

      auto [ r_raw, r_max ] = rand30();

      // Normalize exactly to [0, 1] based on actual generator capability
      double uniform_01 = static_cast<double>(r_raw) / static_cast<double>(r_max);
      return static_cast<T>(M + uniform_01 * (N - M));
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
    @brief Generates a random value between two bounds.
    @ingroup tools-random

    This function generates a random value of type `T` between the specified `mini` and `maxi`
    bounds using an uniform distribution for integral types and a log-uniform distribution for
    floating-point types. The seed is derived from the tts::random_seed function.

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
