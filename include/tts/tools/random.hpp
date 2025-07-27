//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

namespace tts
{
  namespace _
  {
    template<std::integral T> T roll(T M, T N) { return M + rand() / (RAND_MAX / (N - M + 1) + 1); }

    template<std::floating_point T> T roll(T M, T N)
    {
      double base = static_cast<double>((rand() * (N-M)));
      return static_cast<T>(M + base / static_cast<double>(RAND_MAX));
    }

    template<typename T> T exp10(T a) { return exp(log(T(10))*a); }

    template<std::integral T> T roll_random(T mini, T maxi)
    {
      return _::roll(mini, maxi);
    }

    template<std::floating_point T> T roll_random(T mini, T maxi)
    {
      // restrict to finite values in any cases
      constexpr T smvlp  = std::numeric_limits<T>::min();
      constexpr T valmax = std::numeric_limits<T>::max();
      constexpr T eps    = std::numeric_limits<T>::epsilon();
      constexpr T nan    = std::numeric_limits<T>::quiet_NaN();

      if (mini ==    0) mini =  smvlp;
      if (isinf(maxi) && maxi > 0) maxi =  valmax;
      if (isinf(mini) && mini < 0) maxi =  valmax;
      if (maxi ==    0) maxi = -smvlp;

      T value = {};

      if(mini < 0 && maxi > 0)
      {
        auto amn  = _::abs(mini);
        auto amx  = _::abs(maxi);
        auto lamn = log10(amn);
        auto lamx = log10(amx);

        auto s    = _::roll(T{0},amx+amn) < amx;
        if      (lamn >= 0 && log10(amx)  >=  0)  return s ? -roll_random(smvlp, amn)   :  roll_random(smvlp, amx );
        else if (lamn <= 0 && lamx        <=  0)  return s ? -roll_random(smvlp, amn)   :  roll_random(smvlp, amx );
        else if (lamn <= 0 && lamx        >=  0)  return s ? -roll_random(smvlp, amn)   :  roll_random(smvlp, amx );
        else if (lamn >= 0 && lamx        <=  0)  return s ?  roll_random(smvlp, -mini) : -roll_random(smvlp, maxi);
        else                                      return nan;
      }
      else
      {
        if (mini > 0)
        {
          if (mini < 1 && maxi >  1) mini = _::max(T(1)/std::sqrt(maxi), mini);
          mini = (maxi == 1) ? eps : mini;
          value = _::exp10(_::roll(log10(mini), log10(maxi)));
        }
        else if (maxi < 0)
        {
          if (mini < -1 && maxi > -1) maxi = _::min(T(1)/std::sqrt(mini), maxi);
          maxi = (mini == -1) ? -eps : maxi;
          value= -_::exp10(_::roll(log10(-maxi), log10(-mini)));
        }
        return value;
      }
    }
  }

  template<typename T> T random_value(T mini, T maxi)
  {
    return _::roll_random(mini,maxi);
  }
}