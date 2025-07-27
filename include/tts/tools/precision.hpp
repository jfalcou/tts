//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once
#include <tts/tools/bitcast.hpp>

namespace tts
{
  namespace _
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

  //====================================================================================================================
  /*!
    @brief Compute the absolute distance between two values

    This function can be overloaded in the @ref tts namespace to compute the  distance for external types.

    @param  a Value to compare
    @param  b Value to compare
    @return The distance between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double absolute_check(T const &a, U const &b)
  {
    if constexpr( requires { absolute_distance(a,b); }) return absolute_distance(a,b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (_::isnan(a) && _::isnan(b))) return 0.;

        if(_::isinf(a) || _::isinf(b) || _::isnan(a) || _::isnan(b))
          return std::numeric_limits<double>::infinity();

        return _::abs(a - b);
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absolute_check(d0, d1);
      }
      else
      {
        static_assert ( std::is_floating_point_v<T> || std::is_integral_v<T>
                      , "[TTS] TTS_ABSOLUTE_EQUAL requires integral or floating points data to compare."
                        "Did you mean to use TTS_ALL_ABSOLUTE_EQUAL or to overload tts::absolute_check ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absolute_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @brief Compute the relative distance between two values

    This function can be overloaded in the @ref tts namespace to compute the relative distance for external types.

    @param  a Value to compare
    @param  b Value to compare
    @return The relative distance between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double relative_check(T const &a, U const &b)
  {
    if constexpr( requires { relative_distance(a,b); }) return relative_distance(a,b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      { return a == b ? 0. : 100.; }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (_::isnan(a) && _::isnan(a))) return 0.;

        if(_::isinf(a) || _::isinf(b) || _::isnan(a) || _::isnan(b))
          return std::numeric_limits<double>::infinity();

        return 100. * (_::abs(a - b) / _::max(T(1), _::max(_::abs(a), _::abs(b))));
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return relative_check(d0, d1);
      }
      else
      {
        static_assert ( std::is_floating_point_v<T> || std::is_integral_v<T>
                      , "[TTS] TTS_RELATIVE_EQUAL requires integral or floating points data to compare."
                        "Did you mean to use TTS_ALL_RELATIVE_EQUAL or to overload tts::relative_check ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return relative_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @brief Compute the distance in ULP between two values

    This function can be overloaded in the @ref tts namespace to compute the distance in ULP for external types.

    @param  a Value to compare
    @param  b Value to compare
    @return The distance in ULP  between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double ulp_check(T const &a, U const &b)
  {
    if constexpr( requires { ulp_distance(a,b); }) return ulp_distance(a,b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;

        if((a == b) || (_::isnan(a) && _::isnan(b)))
        {
          return 0.;
        }
        else if (_::isunordered(a, b))
        {
          return std::numeric_limits<double>::infinity();
        }
        else
        {
          auto aa = _::bitinteger(a);
          auto bb = _::bitinteger(b);

          if(aa > bb) std::swap(aa, bb);

          auto z = static_cast<ui_t>(bb-aa);

          if( _::signbit(a) != _::signbit(b) ) ++z;
          return z/2.;
        }
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        using u_t = typename std::make_unsigned<T>::type;

        // TODO: Fix overflow in case of very huge integral value
        return ((a < b) ? u_t(b - a) : u_t(a - b))/2.;
      }
      else
      {
        static_assert ( std::is_floating_point_v<T> || std::is_integral_v<T>
                      , "[TTS] TTS_ULP_EQUAL requires integral or floating points data to compare."
                        "Did you mean to use TTS_ALL_ULP_EQUAL or to overload tts::ulp_check ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulp_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @brief Compute if values are exactly equals or all NaNs/Invalids

    This function can be overloaded in the @ref tts namespace to compute the strict equality for external types.

    @param  a Value to compare
    @param  b Value to compare
    @return Is `a == b` or `_::isnan(a) && _::isnan(b)`
  **/
  //====================================================================================================================
  template<typename T, typename U> inline bool ieee_check(T const &a, U const &b)
  {
    if constexpr( requires { ieee_equal(a,b); }) return ieee_equal(a,b);
    else if constexpr(std::is_floating_point_v<T>) // IEEE cases
    {
      return (a==b) || (_::isnan(a) && _::isnan(b));
    }
    else
    {
      return a == b;
    }
  }
}
