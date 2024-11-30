//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/bitcast.hpp>
#include <type_traits>
#include <limits>
#include <cmath>


namespace tts
{
namespace detail
{
  #if defined(__FAST_MATH__)
    inline constexpr auto isinf = [](auto) { return false; };
    inline constexpr auto isnan = [](auto) { return false; };
  #else
    inline constexpr auto isinf = [](auto x) { return std::isinf(x); };
    inline constexpr auto isnan = [](auto x) { return std::isnan(x); };
  #endif
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
  template<typename T, typename U> inline double absolute_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (detail::isnan(a) && detail::isnan(b))) return 0.;

        if(detail::isinf(a) || detail::isinf(b) || detail::isnan(a) || detail::isnan(b))
          return std::numeric_limits<double>::infinity();

        return std::abs(a - b);
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absolute_distance(d0, d1);
      }
      else
      {
        static_assert ( std::is_floating_point_v<T> || std::is_integral_v<T>
                      , "[TTS] TTS_ABSOLUTE_EQUAL requires integral or floating points data to compare."
                        "Did you mean to use TTS_ALL_ABSOLUTE_EQUAL or to overload tts::absolute_distance ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absolute_distance(static_cast<common_t>(a), static_cast<common_t>(b));
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
  template<typename T, typename U> inline double relative_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      { return a == b ? 0. : 100.; }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (detail::isnan(a) && detail::isnan(b))) return 0.;

        if(detail::isinf(a) || detail::isinf(b) || detail::isnan(a) || detail::isnan(b))
          return std::numeric_limits<double>::infinity();

        return 100. * (std::abs(a - b) / std::max(T(1), std::max(std::abs(a), std::abs(b))));
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return relative_distance(d0, d1);
      }
      else
      {
        static_assert ( std::is_floating_point_v<T> || std::is_integral_v<T>
                      , "[TTS] TTS_RELATIVE_EQUAL requires integral or floating points data to compare."
                        "Did you mean to use TTS_ALL_RELATIVE_EQUAL or to overload tts::relative_distance ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return relative_distance(static_cast<common_t>(a), static_cast<common_t>(b));
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
  template<typename T, typename U> inline double ulp_distance(T const &a, U const &b)
  {
    if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;

        if((a == b) || (detail::isnan(a) && detail::isnan(b)))
        {
          return 0.;
        }
        else if (std::isunordered(a, b))
        {
          return std::numeric_limits<double>::infinity();
        }
        else
        {
          auto aa = detail::bitinteger(a);
          auto bb = detail::bitinteger(b);

          if(aa > bb) std::swap(aa, bb);

          auto z = static_cast<ui_t>(bb-aa);

          if( std::signbit(a) ^ std::signbit(b) ) ++z;
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
                        "Did you mean to use TTS_ALL_ULP_EQUAL or to overload tts::ulp_distance ?"
                      );
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulp_distance(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }


  //====================================================================================================================
  /*!
    @brief Compute if values are exactly equals or all NaNs/Invalids

    This function can be overloaded in the @ref tts namespace to compute the strict equality for external types.

    @param  a Value to compare
    @param  b Value to compare
    @return Is `a == b` or `detail::isnan(a) && detail::isnan(b)`
  **/
  //====================================================================================================================
  template<typename T, typename U> inline bool is_ieee_equal(T const &a, U const &b)
  {
    if constexpr(std::is_floating_point_v<T>) // IEEE cases
    {
      return (a==b) || (detail::isnan(a) && detail::isnan(b));
    }
    else
    {
      return a == b;
    }
  }
}
